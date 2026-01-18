#pragma once
#include <cstdint>
#include <position/position.h>
#include <move/move.h>
#include <move_search.h>

namespace fork {

	class Engine {
	public:

		/*
		*@brief Returns a double representing the strength of the position for the white player.
		* Higher values are better, and zero represents an equal position.
		* @param position : Chess position.
		* @return double
		*/
		double eval(core::Position& position);

		/**
		 * @brief Searches the current position and selects the best move.
		 * @param position: Reference to the current chess position to be searched.
		 *                 The position may be temporarily modified during the search
		 *                 but will be restored before the function returns.
		 * @return move
		 */
		move::Move search(core::Position& position);
	};
}


