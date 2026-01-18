#pragma once

#include <move/move.h>
#include <position/position.h>


namespace move {

	/**
	 * @brief Searches the current position and selects the best move.
	 * @param position: Reference to the current chess position to be searched.
	 *                 The position may be temporarily modified during the search
	 *                 but will be restored before the function returns.
	 * @return move
	 */
	move::Move search(core::Position& position);
}