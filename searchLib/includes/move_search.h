#pragma once

#include <move/move.h>
#include <position/position.h>


namespace move::search {

	/**
	 * @brief Searches the current position and selects the best move.
	 *
	 * Performs a move search on the given chess position using the engine's
	 * evaluation and search algorithms (e.g., minimax, alpha-beta pruning).
	 * The position is analyzed from the side to move, and the most favorable
	 * legal move is returned.
	 *
	 * @param position: Reference to the current chess position to be searched.
	 *                 The position may be temporarily modified during the search
	 *                 but will be restored before the function returns.
	 *
	 * @return move
	 */
	move::Move search_moves(chess::Position& position);
}