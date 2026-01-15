#pragma once
#include <position/position.h>
#include <cmath>

namespace position::evaluation {

	constexpr double pawnValue = 1;
	constexpr double knightValue = 3;
	constexpr double bishopValue = 3;
	constexpr double rookValue = 5;
	constexpr double queenValue = 9;
	constexpr double kingValue = INT32_MAX;

	/*
	* @brief Returns a double representing the strength of the position for the white player.
	* Higher values are better, and zero represents an equal position.
	* @param position: Chess position.
	* @return double
	*/
	double eval(chess::Position& position);
}