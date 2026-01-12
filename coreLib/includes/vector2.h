#pragma once

#include <cmath>

namespace util {
	
	class Vector2 {

	public:
		double x, y;

		inline Vector2(double _x, double _y) : x(_x), y(_y) { }
		inline Vector2() : x(0), y(0) { };
		inline ~Vector2() = default;

		inline void add(double _x, double _y) {
			x += _x; y += _y;
		}

		inline void add(Vector2& v) {
			x += v.x; y += v.y;
		}

		inline double dot(double _x, double _y) {
			return x * _x + y * _y;
		}

		inline double dot(Vector2& v) {
			return x * v.x + y * v.y;
		}

		inline double mag() {
			return sqrt(x * x + y * y);
		}
	};
}