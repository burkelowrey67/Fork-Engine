#include <move/move_lookup.h>
#include <move/magic_numbers.h>
#include <position/bit_board.h>
#include <move/movement_masks.h>
#include <bit>
#include <vector>

namespace move::lookups {

	static uint64_t bishopLookups[64][512];
	static uint64_t rookLookups[64][4096];

	uint64_t lookup(piece::PieceType pieceType, unsigned int squareIndex, uint64_t allPieces) {
		switch (pieceType) {
		case piece::PieceType::Knight:  return move::masks::KNIGHT_MASKS[squareIndex];
		case piece::PieceType::Bishop:  return bishopLookups[squareIndex][magic_numbers::get_lookup_index(allPieces & move::masks::BISHOP_MASKS[squareIndex], squareIndex, pieceType)];
		case piece::PieceType::Rook:    return rookLookups[squareIndex][magic_numbers::get_lookup_index(allPieces & move::masks::ROOK_MASKS[squareIndex], squareIndex, pieceType)];
		case piece::PieceType::Queen:
			return  bishopLookups[squareIndex][magic_numbers::get_lookup_index(allPieces & move::masks::BISHOP_MASKS[squareIndex], squareIndex, pieceType)] |
					rookLookups[squareIndex][magic_numbers::get_lookup_index(allPieces & move::masks::BISHOP_MASKS[squareIndex], squareIndex, pieceType)];
		case piece::PieceType::King:    return move::masks::KING_MASKS[squareIndex];
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
			chess::bit_board::remove_first_one(movementMask);
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

	static void remove_redundant_edge_bits(uint64_t& bitBoard, unsigned int currentSquare, piece::PieceType pieceType) {
		if (pieceType == piece::PieceType::Rook) {
			if ((currentSquare & chess::bit_board::FIRST_RANK) != 0) {
				bitBoard &= ~(
					chess::bit_board::EIGHTH_RANK	| 
					chess::bit_board::SW_CORNER		| 
					chess::bit_board::SE_CORNER
					);
			}

			if ((currentSquare & chess::bit_board::EIGHTH_RANK) != 0) {
				bitBoard &= ~(
					chess::bit_board::FIRST_RANK	| 
					chess::bit_board::NW_CORNER		| 
					chess::bit_board::NE_CORNER
					);
			}

			if ((currentSquare & chess::bit_board::A_FILE) != 0) {
				bitBoard &= ~(
					chess::bit_board::H_FILE		|
					chess::bit_board::NW_CORNER		|
					chess::bit_board::SW_CORNER
					);
			}

			if ((currentSquare & chess::bit_board::H_FILE) != 0) {
				bitBoard &= ~(
					chess::bit_board::A_FILE		| 
					chess::bit_board::NE_CORNER		| 
					chess::bit_board::SE_CORNER
					);
			}
		}

		else {
			bitBoard ^= chess::bit_board::EDGE_MASK;
		}
	}

	template
	<size_t N>
	static void initialize_lookup(piece::PieceType pieceType, uint64_t(&lookup)[64][N]) {
		for (int squareIndex = 0; squareIndex < 64; squareIndex++) {
			uint64_t movementMask;

			if (pieceType == piece::PieceType::Bishop || pieceType == piece::PieceType::Rook) {
				movementMask = move::masks::get_mask(pieceType, squareIndex);
			}
			else return;

			remove_redundant_edge_bits(movementMask, squareIndex, pieceType);
			int hashBucketSize = pieceType == piece::PieceType::Bishop ? 512 : 4096;
			if (hashBucketSize != N) return;

			std::vector<uint64_t> blockerConfigs = create_blocker_configs(movementMask);

			for (uint64_t blockerConfig : blockerConfigs) {
				int lookupIndex = move::magic_numbers::get_lookup_index(blockerConfig, squareIndex, pieceType);
				lookup[squareIndex][lookupIndex] = move::masks::get_mask(pieceType, squareIndex);
			}
		}
	}

	void initialize_lookups() {
		initialize_lookup<512>(piece::PieceType::Bishop, bishopLookups);
		initialize_lookup<4096>(piece::PieceType::Rook, rookLookups);
	}
}