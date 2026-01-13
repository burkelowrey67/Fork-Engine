#pragma once

#include <move/move.h>
#include <position/position.h>
#include <vector>

namespace move::generation {

	/*
	* @brief Given a position and a move list, it generates psuedo-legal moves and appends them to the list. 
	* Note: Does not check for position-legality.
	* @param position:	Chess position.
	* @param moves:		[Out] Output container filled with moves.
	*/
	void generate_pseudolegal_moves(chess::Position& position, std::vector<uint32_t>& moves);
}