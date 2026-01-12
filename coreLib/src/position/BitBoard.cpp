#include <position/bit_board.h>
#include <bit>

namespace position::bit_board {

	void remove_redundant_edge_bits(uint64_t& bitBoard, unsigned int currentSquare, piece::PieceType pieceType) {
		if (pieceType == piece::PieceType::Rook) {
			if ((currentSquare & FIRST_RANK) != 0) bitBoard &= ~(EIGHTH_RANK | SW_CORNER | SE_CORNER);
			if ((currentSquare & EIGHTH_RANK) != 0) bitBoard &= ~(FIRST_RANK | NW_CORNER | NE_CORNER);
			if ((currentSquare & A_FILE) != 0) bitBoard &= ~(H_FILE | NW_CORNER | SW_CORNER);
			if ((currentSquare & H_FILE) != 0) bitBoard &= ~(A_FILE | NE_CORNER | SE_CORNER);
		}

		else {
			bitBoard ^= EDGE_MASK;
		}
	}

	uint64_t square_to_bit_board(unsigned int squareIndex) {
		return squareIndex < 64
			? (1ULL << squareIndex)
			: 0ULL;
	}

	unsigned int get_first_square_index(uint64_t& bitBoard) {
		return std::countr_zero(bitBoard);
	}

	void move_bit(uint64_t& bitBoard, unsigned int startSquare, unsigned int endSquare) {
		uint64_t startMask = square_to_bit_board(startSquare);
		uint64_t endMask = square_to_bit_board(endSquare);

		// Check if start bit is set
		if ((bitBoard & startMask) == 0) {
			// No bit to move — return unchanged or throw exception
			return;
		}

		bitBoard &= ~startMask; // clear start
		bitBoard |= endMask;    // set end
	}

	void set_bit_one(uint64_t& bitBoard, unsigned int squareIndex) {
		bitBoard |= square_to_bit_board(squareIndex);
	}


	void set_bit_zero(uint64_t& bitBoard, unsigned int squareIndex) {
		bitBoard &= ~square_to_bit_board(squareIndex);
	}

	void remove_first_bit(uint64_t& bitBoard) {
		bitBoard &= bitBoard - 1;
	}
}