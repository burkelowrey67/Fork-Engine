#pragma once
#include <color.h>

namespace core::piece {
	enum class PieceType : uint8_t {
		Pawn, Knight, Bishop, Rook, Queen, King, 
		None, N
	};
}