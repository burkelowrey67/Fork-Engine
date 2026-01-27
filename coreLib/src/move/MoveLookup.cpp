#include <move/move_lookup.h>
#include <move/magic_numbers.h>
#include <position/bit_board.h>
#include <move/movement_masks.h>
#include <bit>
#include <vector>
#include <iostream>

namespace move::mask {

	static uint64_t bishopLookups[64][2048];
	static uint64_t rookLookups[64][16384];

	static int bishopDirs[4][2] =	{ { 1, 1 }, { 1, -1 }, { -1, -1 }, { -1, 1 } };
	static int rookDirs[4][2] =		{ { 0, 1 }, { 1, 0 }, { 0, -1 }, { -1, 0 } };

	static uint64_t remove_redundant_edge_bits(uint64_t bitBoard, unsigned int currentSquare, core::PieceType pieceType) {
		uint64_t mask = core::bit_board::square_to_bit_board(currentSquare);

		if (pieceType == core::PieceType::Rook) {
			if (mask & core::bit_board::SW_CORNER || mask & core::bit_board::NE_CORNER) {
				bitBoard &= ~(core::bit_board::NW_CORNER | core::bit_board::SE_CORNER);
			}

			else if (mask & core::bit_board::NW_CORNER || mask & core::bit_board::SE_CORNER) {
				bitBoard &= ~(core::bit_board::SW_CORNER | core::bit_board::NE_CORNER);
			}

			else if (mask & core::bit_board::FIRST_RANK) {
				bitBoard &= ~(
					core::bit_board::EIGHTH_RANK |
					core::bit_board::A_FILE |
					core::bit_board::H_FILE
					);
			}

			else if (mask & core::bit_board::EIGHTH_RANK) {
				bitBoard &= ~(
					core::bit_board::FIRST_RANK |
					core::bit_board::A_FILE |
					core::bit_board::H_FILE
					);
			}

			else if (mask & core::bit_board::A_FILE) {
				bitBoard &= ~(
					core::bit_board::H_FILE |
					core::bit_board::FIRST_RANK |
					core::bit_board::EIGHTH_RANK
					);
			}

			else if (mask & core::bit_board::H_FILE) {
				bitBoard &= ~(
					core::bit_board::A_FILE |
					core::bit_board::FIRST_RANK |
					core::bit_board::EIGHTH_RANK
					);
			}

			else bitBoard &= ~core::bit_board::EDGE_MASK;
		}

		else bitBoard &= ~core::bit_board::EDGE_MASK;

		return bitBoard;
	}

	uint64_t lookup(core::PieceType pieceType, unsigned int squareIndex, uint64_t allPieces, uint64_t friendlyPieces) {
		switch (pieceType) {
		case core::PieceType::Knight: return move::mask::KNIGHT_MASKS[squareIndex] & ~friendlyPieces;

		case core::PieceType::Bishop:
			return bishopLookups[squareIndex][magic_numbers::get_lookup_index(remove_redundant_edge_bits(allPieces & move::mask::BISHOP_MASKS[squareIndex], squareIndex, core::PieceType::Bishop), squareIndex, pieceType)] & ~friendlyPieces;

		case core::PieceType::Rook:
			return rookLookups[squareIndex][magic_numbers::get_lookup_index(remove_redundant_edge_bits(allPieces & move::mask::ROOK_MASKS[squareIndex], squareIndex, core::PieceType::Rook), squareIndex, pieceType)] & ~friendlyPieces;

		case core::PieceType::Queen:
			return  (bishopLookups[squareIndex][magic_numbers::get_lookup_index(remove_redundant_edge_bits(allPieces & move::mask::BISHOP_MASKS[squareIndex], squareIndex, core::PieceType::Bishop), squareIndex, core::PieceType::Bishop)] |
					 rookLookups[squareIndex][magic_numbers::get_lookup_index(remove_redundant_edge_bits(allPieces & move::mask::ROOK_MASKS[squareIndex], squareIndex, core::PieceType::Rook), squareIndex, core::PieceType::Rook)]) & ~friendlyPieces;

		case core::PieceType::King: return move::mask::KING_MASKS[squareIndex] & ~friendlyPieces;
		default: return 0;
		}
	}

	static std::vector<uint64_t> create_blocker_configs(uint64_t movementMask) {

		int numSquareIndices = std::popcount(movementMask);
		std::vector<int> moveSquareIndices;
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

			for (size_t i = 0; i < moveSquareIndices.size(); i++) {
				uint64_t bit = (config >> i) & 1L; // explicitly 0 or 1
				patternBitBoard |= (bit << moveSquareIndices[i]);
			}

			blockerConfigs.push_back(patternBitBoard);
		}

		return blockerConfigs;
	}

	static uint64_t ray_cast(int squareIndex, core::PieceType pieceType, uint64_t blockers) {
		uint64_t attacks = 0ULL;

		int startFile = squareIndex & 0b111;
		int startRank = squareIndex >> 3;

		auto dirs = pieceType == core::PieceType::Bishop ? bishopDirs : rookDirs;

		for (int i = 0; i < 4; i++) {
			int file = startFile + dirs[i][0];
			int rank = startRank + dirs[i][1];

			// step in this direction
			while (file >= 0 && file < 8 && rank >= 0 && rank < 8) {
				int sq = (rank << 3) | file;
				uint64_t bit = 1ULL << sq;

				attacks |= bit;

				// if ray must stop when encountering a blocker
				if (blockers & bit) break;

				file += dirs[i][0];
				rank += dirs[i][1];
			}
		}

		return attacks;
	}

	template
	<size_t N>
	static void initialize_lookup(core::PieceType pieceType, uint64_t(&lookup)[64][N]) {
		if (pieceType != core::PieceType::Bishop && pieceType != core::PieceType::Rook) return;

		for (int squareIndex = 0; squareIndex < 64; squareIndex++) {
			uint64_t movementMask = move::mask::get_mask(pieceType, squareIndex);

			movementMask = remove_redundant_edge_bits(movementMask, squareIndex, pieceType);

			std::vector<uint64_t> blockerConfigs = create_blocker_configs(movementMask);

			for (uint64_t blockerConfig : blockerConfigs) {
				int lookupIndex = move::magic_numbers::get_lookup_index(blockerConfig, squareIndex, pieceType);
				lookup[squareIndex][lookupIndex] = ray_cast(squareIndex, pieceType, blockerConfig);
			}
		}
	}

	void initialize_lookups() {
		initialize_lookup<2048>(core::PieceType::Bishop, bishopLookups);
		initialize_lookup<16384>(core::PieceType::Rook, rookLookups);
	}
}