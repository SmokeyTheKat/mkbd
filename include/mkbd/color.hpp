#ifndef __MKBD_COLOR_HPP__
#define __MKBD_COLOR_HPP__

#define RGB_ARGS(c) c.r, c.g, c.b

#include <mkbd/utils.hpp>
#include <mkbd/math.hpp>
#include <algorithm>

struct Color {
	int r, g, b;
	Color(int r, int g, int b) : r(r), g(g), b(b) {};
	Color(int v) : r(v), g(v), b(v) {};
	Color(void) : r(0), g(0), b(0) {};

	static inline Color none(void) {
		return Color(-1, -1, -1);
	}

	inline Color invert(void) const {
		return Color(
			rmap(r, 0, 255, 255, 0),
			rmap(g, 0, 255, 255, 0),
			rmap(b, 0, 255, 255, 0)
		);
	}

	inline Color darken(double perc = 0.2) const {
		double value = 1.0 - perc;
		return Color(r * value, g * value, b * value);
	}

	inline Color lighten(void) const {
		return Color(
			std::min(255, (int)(1.4 * r)),
			std::min(255, (int)(1.4 * g)),
			std::min(255, (int)(1.4 * b))
		);
	}

	inline Color augment(void) const {
		int avg = (r + g + b) / 3;
		if (avg > 128) {
			return this->lighten();
		} else {
			return this->darken();
		}
	}

	bool isColor(void) {
		return r >= 0 && g >= 0 && b >= 0;
	}

	bool operator==(const Color& other) const {
		return (
			r == other.r &&
			g == other.g &&
			b == other.b
		);
	}
};

namespace Colors {
	static const Color Black(0, 0, 0);
	static const Color White(255, 255, 255);
	static const Color Red(255, 0, 0);
	static const Color Green(0, 255, 0);
	static const Color Blue(0, 0, 255);
};

#endif