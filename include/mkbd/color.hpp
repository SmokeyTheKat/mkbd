#ifndef __MKBD_COLOR_HPP__
#define __MKBD_COLOR_HPP__

#define RGB_ARGS(c) c.r, c.g, c.b

#include <mkbd/utils.hpp>
#include <mkbd/math.hpp>
#include <algorithm>
#include <iostream>
#include <ostream>

enum class ColorFormat {
	Rgb,
	Hsv
};

struct Color {
	union {
		struct {
			int r, g, b;
		};
		struct {
			int h, s, v;
		};
	};

	Color(int r, int g, int b) : r(r), g(g), b(b) {};
	Color(int a, int b, int c, ColorFormat format) {
		if (format == ColorFormat::Rgb) {
			r = a;
			g = b;
			b = c;
		} else if (format == ColorFormat::Hsv) {
			h = a;
			s = b;
			v = c;
		}
	}
	Color(int v) : r(v), g(v), b(v) {};
	Color(void) : r(0), g(0), b(0) {};

	void toRgb(void) {
		double nh = h;
		double ns = s / 100.0;
		double nv = v / 100.0;

		double c = nv * ns;
		double x = c * (1.0 - std::abs(std::fmod(nh / 60.0, 2.0) - 1.0));
		double m = nv - c;

		double nr, ng, nb;

		if (nh < 60.0) {
			nr = c;
			ng = x;
			nb = 0;
		} else if (nh < 120.0) {
			nr = x;
			ng = c;
			nb = 0;
		} else if (nh < 180.0) {
			nr = 0;
			ng = c;
			nb = x;
		} else if (nh < 240.0) {
			nr = 0;
			ng = x;
			nb = c;
		} else if (nh < 300.0) {
			nr = x;
			ng = 0;
			nb = c;
		} else if (nh < 360.0) {
			nr = c;
			ng = 0;
			nb = x;
		}

		nr = (nr + m) * 255.0;
		ng = (ng + m) * 255.0;
		nb = (nb + m) * 255.0;

		r = nr;
		g = ng;
		b = nb;
	}

	void toHsv(void) {
		double nr = r / 255.0;
		double ng = g / 255.0;
		double nb = b / 255.0;

		double max = std::max({nr, ng, nb});
		double min = std::min({nr, ng, nb});
		double d = max - min;

		double nh, ns, nv;

		if (d == 0) {
			nh = 0.0;
		} else if (max == nr) {
			nh = 60.0 * std::fmod((ng - nb) / d, 6.0);
		} else if (max == ng) {
			nh = 60.0 * ((nb - nr) / d + 2.0);
		} else if (max == nb) {
			nh = 60.0 * ((nr - ng) / d + 4.0);
		}

		if (max == 0) {
			ns = 0;
		} else {
			ns = d / max;
		}

		nv = max;

		h = nh;
		s = ns * 100.0;
		v = nv * 100.0;
	}

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


inline std::ostream &operator<<(std::ostream &os, Color const& c) { 
	return os << "(" << c.r << ", " << c.g << ", " << c.b << ")";
}

namespace Colors {
	static const Color Black(0, 0, 0);
	static const Color White(255, 255, 255);
	static const Color Red(255, 0, 0);
	static const Color Green(0, 255, 0);
	static const Color Blue(0, 0, 255);
	static const Color Yellow(255, 255, 0);
};

#endif