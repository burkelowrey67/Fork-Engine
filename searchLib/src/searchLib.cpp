// searchLib.cpp : Defines the functions for the static library.


#include "pch.h"
#include <move_search.h>
#include <move/move_generation.h>
#include <move/move_application.h>
#include <eval.h>
#include <vector>
#include <cfloat>

namespace move {


	move::Move search(chess::Position& position) {
		std::vector<uint32_t> moves;
		moves.reserve(256);

		move::generate_pseudolegal_moves(position, moves);
		
		move::Move bestMove;
		double bestScore = DBL_MIN;

		for (move::Move move : moves) {
			chess::Position nextPosition = move::next_position(position, move);
			double score = position::evaluation::eval(nextPosition);
			if (score > bestScore) bestMove = move; bestScore = score;
		}

		return bestMove;
	}
}

