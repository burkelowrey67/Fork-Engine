#pragma once
#include <cstdint>

namespace chess {
	enum class Color : uint8_t{
		White, Black
	};

	constexpr inline Color opposite_color(Color& color) {
		switch (color)
		{
		case Color::White: return Color::Black;
		case Color::Black: return Color::White;
		default: return Color::White;
		}
	}
}