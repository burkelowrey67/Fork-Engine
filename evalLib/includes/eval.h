#pragma once
#include <position/position.h>
#include <cmath>

namespace evaluation {

	constexpr double pawnValue = 1;
	constexpr double knightValue = 3;
	constexpr double bishopValue = 3;
	constexpr double rookValue = 5;
	constexpr double queenValue = 9;
	constexpr double kingValue = INT32_MAX;

	double eval(chess::Position position);
}