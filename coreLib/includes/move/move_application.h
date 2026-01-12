#pragma once

#include <position/position.h>
#include <move/move.h>

namespace move::move_application {

	position::Position next_position(position::Position&, const move::Move&);
	void apply_move(position::Position&, const move::Move&);
}