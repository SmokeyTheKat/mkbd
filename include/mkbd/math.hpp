#ifndef __MKBD_MATH_HPP__
#define __MKBD_MATH_HPP__

#include <algorithm>
#include <limits>
#include <cmath>

double rmap(double v, double l1, double h1, double l2, double h2);
double roundTo(double value, double to);
bool isAbout(double value, double about, double threshold = 0.01);
double freqToRad(double freq);

constexpr double logn(double value, double base) {
	return std::log(value) / std::log(base);
}

template<class B, class S>
S shrink(B value) {
	return std::clamp(value, (B)std::numeric_limits<S>::min(), (B)std::numeric_limits<S>::max());
}

#endif