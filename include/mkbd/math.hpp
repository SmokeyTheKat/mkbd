#ifndef __MKBD_MATH_HPP__
#define __MKBD_MATH_HPP__

#include <algorithm>
#include <limits>
#include <cmath>

double rmap(double v, double l1, double h1, double l2, double h2);
double qlmap(double v, double l1, double h1, double l2, double h2);
double qhmap(double v, double l1, double h1, double l2, double h2);
double roundTo(double value, double to);
bool isAbout(double value, double about, double threshold = 0.01);
double freqToRad(double freq);
double lerp(double a, double b, double t);

constexpr double logn(double value, double base) {
	return std::log(value) / std::log(base);
}

template<class T>
T clamp(T value, T low, T high) {
	value = std::min(value, high);
	value = std::max(value, low);
	return value;
}

template<class B, class S>
S shrink(B value) {
	return clamp(value, (B)std::numeric_limits<S>::min(), (B)std::numeric_limits<S>::max());
}

#endif