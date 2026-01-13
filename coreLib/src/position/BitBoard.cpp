#include <position/bit_board.h>
#include <bit>

namespace chess::bit_board {



	uint64_t square_to_bit_board(unsigned int squareIndex) {
		return squareIndex < 64
			? (1ULL << squareIndex)
			: 0ULL;
	}

	int get_first_square_index(uint64_t& bitBoard) {
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

	void remove_first_one(uint64_t& bitBoard) {
		bitBoard &= bitBoard - 1;
	}
}