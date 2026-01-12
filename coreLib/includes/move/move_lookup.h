#pragma once
#include <cstdint>
#include <piece/piece_type.h>
#include <move/movement_masks.h>

namespace move::lookups {

	void initialize_lookups();
	uint64_t lookup(piece::PieceType, unsigned int squareIndex, uint64_t blockerConfig);

}