#pragma once
#include <position/position.h>
#include <cmath>

namespace position::evaluation {

	inline constexpr double pawnValue = 1;
	inline constexpr double knightValue = 3;
	inline constexpr double bishopValue = 3;
	inline constexpr double rookValue = 5;
	inline constexpr double queenValue = 9;
	inline constexpr double kingValue = INT32_MAX;

	/*
	* @brief Returns a double representing the strength of the position for the white player.
	* Higher values are better, and zero represents an equal position.
	* @param position: Chess position.
	* @return double
	*/
	double eval(core::Position& position);
}