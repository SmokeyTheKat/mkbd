#ifndef __MKBD_COLOR_HPP__
#define __MKBD_COLOR_HPP__

#define RGB_ARGS(c) c.r, c.g, c.b

struct Color {
	int r, g, b;
	inline Color(int r, int g, int b) : r(r), g(g), b(b) {};
	inline Color(void) : r(0), g(0), b(0) {};

	static inline Color none(void) {
		return Color(-1, -1, -1);
	}

	inline bool isColor(void) {
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
	static Color Black(0, 0, 0);
	static Color White(255, 255, 255);
};

#endif