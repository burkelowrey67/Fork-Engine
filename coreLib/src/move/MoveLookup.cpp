#include <move/move_lookup.h>
#include <move/magic_numbers.h>
#include <position/bit_board.h>
#include <move/movement_masks.h>
#include <bit>
#include <vector>

namespace move::mask {

	static uint64_t bishopLookups[64][512];
	static uint64_t rookLookups[64][4096];

	uint64_t lookup(core::PieceType pieceType, unsigned int squareIndex, uint64_t allPieces) {
		switch (pieceType) {
		case core::PieceType::Knight:  return move::mask::KNIGHT_MASKS[squareIndex];
		case core::PieceType::Bishop:  return bishopLookups[squareIndex][magic_numbers::get_lookup_index(allPieces & move::mask::BISHOP_MASKS[squareIndex], squareIndex, pieceType)];
		case core::PieceType::Rook:    return rookLookups[squareIndex][magic_numbers::get_lookup_index(allPieces & move::mask::ROOK_MASKS[squareIndex], squareIndex, pieceType)];
		case core::PieceType::Queen:
			return  bishopLookups[squareIndex][magic_numbers::get_lookup_index(allPieces & move::mask::BISHOP_MASKS[squareIndex], squareIndex, pieceType)] |
					rookLookups[squareIndex][magic_numbers::get_lookup_index(allPieces & move::mask::BISHOP_MASKS[squareIndex], squareIndex, pieceType)];
		case core::PieceType::King:    return move::mask::KING_MASKS[squareIndex];
		default: return 0;
		}
	}

	static std::vector<uint64_t> create_blocker_configs(uint64_t movementMask) {

		int numSquareIndices = std::popcount(movementMask);
		std::vector<uint64_t> moveSquareIndices;
		moveSquareIndices.reserve(numSquareIndices);

		while (movementMask) {
			int index = std::countr_zero(movementMask);
			moveSquareIndices.push_back(index);
			core::bit_board::remove_first_one(movementMask);
		}

		int numConfigs = 1 << numSquareIndices;
		std::vector<uint64_t> blockerConfigs;
		blockerConfigs.reserve(numConfigs);

		for (int config = 0; config < numConfigs; config++) {
			uint64_t patternBitBoard = 0;

			for (int i = 0; i < moveSquareIndices.size(); i++) {
				uint64_t bit = (config >> i) & 1L; // explicitly 0 or 1
				patternBitBoard |= (bit << moveSquareIndices[i]);
			}

			blockerConfigs.push_back(patternBitBoard);
		}

		return blockerConfigs;
	}

	static void remove_redundant_edge_bits(uint64_t& bitBoard, unsigned int currentSquare, core::PieceType pieceType) {
		if (pieceType == core::PieceType::Rook) {
			if ((currentSquare & core::bit_board::FIRST_RANK) != 0) {
				bitBoard &= ~(
					core::bit_board::EIGHTH_RANK	| 
					core::bit_board::SW_CORNER		| 
					core::bit_board::SE_CORNER
					);
			}

			if ((currentSquare & core::bit_board::EIGHTH_RANK) != 0) {
				bitBoard &= ~(
					core::bit_board::FIRST_RANK	| 
					core::bit_board::NW_CORNER		| 
					core::bit_board::NE_CORNER
					);
			}

			if ((currentSquare & core::bit_board::A_FILE) != 0) {
				bitBoard &= ~(
					core::bit_board::H_FILE		|
					core::bit_board::NW_CORNER		|
					core::bit_board::SW_CORNER
					);
			}

			if ((currentSquare & core::bit_board::H_FILE) != 0) {
				bitBoard &= ~(
					core::bit_board::A_FILE		| 
					core::bit_board::NE_CORNER		| 
					core::bit_board::SE_CORNER
					);
			}
		}

		else {
			bitBoard ^= core::bit_board::EDGE_MASK;
		}
	}

	template
	<size_t N>
	static void initialize_lookup(core::PieceType pieceType, uint64_t(&lookup)[64][N]) {
		for (int squareIndex = 0; squareIndex < 64; squareIndex++) {
			uint64_t movementMask;

			if (pieceType == core::PieceType::Bishop || pieceType == core::PieceType::Rook) {
				movementMask = move::mask::get_mask(pieceType, squareIndex);
			}
			else return;

			remove_redundant_edge_bits(movementMask, squareIndex, pieceType);
			int hashBucketSize = pieceType == core::PieceType::Bishop ? 512 : 4096;
			if (hashBucketSize != N) return;

			std::vector<uint64_t> blockerConfigs = create_blocker_configs(movementMask);

			for (uint64_t blockerConfig : blockerConfigs) {
				int lookupIndex = move::magic_numbers::get_lookup_index(blockerConfig, squareIndex, pieceType);
				lookup[squareIndex][lookupIndex] = move::mask::get_mask(pieceType, squareIndex);
			}
		}
	}

	void initialize_lookups() {
		initialize_lookup<512>(core::PieceType::Bishop, bishopLookups);
		initialize_lookup<4096>(core::PieceType::Rook, rookLookups);
	}
}