// evalLib.cpp : Defines the functions for the static library.

#include "pch.h"
#include <eval.h>

namespace position::evaluation {

	double eval(core::Position& position) {
		return
			pawnValue	* position.pawn_diff() +
			knightValue * position.knight_diff() +
			bishopValue * position.bishop_diff() +
			rookValue	* position.rook_diff() +
			queenValue	* position.queen_diff();
	}
}
