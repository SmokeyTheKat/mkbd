#include <mkbd/math.hpp>

#include <cmath>

double rmap(double v, double l1, double h1, double l2, double h2) {
	return (v-l1) * ((h2-l2)/(h1-l1)) + l2;
}

double roundTo(double value, double to) {
	double mod = std::fmod(value, to);
	if (mod / to < 0.5)
		return value - std::fmod(value, to);
	else return value + (to - std::fmod(value, to));
}
