#pragma once
#include <cstdint>

namespace core {
	enum class Color : uint8_t{
		White, Black
	};

	/*
	* @brief Returns the opposing color.
	* @param color
	*/
	constexpr inline Color opposite_color(Color& color) {
		switch (color)
		{
		case Color::White: return Color::Black;
		case Color::Black: return Color::White;
		}
	}
}