#pragma once
#include <cstdint>
#include <piece/piece_type.h>
#include <move/movement_masks.h>

namespace move::lookups {

	/*
	* @brief initializes the look-up with computed bit boards.
	*/
	void initialize_lookups();

	/*
	* @brief Returns a movement mask. Note: Bit board - LSB = A1, MSB = H8
	* @param pieceType:		Type of piece specifying look-up.
	* @param squareIndex:	Index of a square in a bit board [0, 63].
	* @param blockerConfig:	Bit board of all pieces in the path of the piece.
	* @return bit board
	*/
	uint64_t lookup(piece::PieceType pieceType, unsigned int squareIndex, uint64_t blockerConfig);

}