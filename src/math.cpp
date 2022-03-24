#include <mkbd/math.hpp>

#include <cmath>

double rmap(double v, double l1, double h1, double l2, double h2) {
	return (v-l1) * ((h2-l2)/(h1-l1)) + l2;
}

double logn(double value, double base) {
	return std::log(value) / std::log(base);
}

double roundTo(double value, double to) {
	return value - std::fmod(value, to);
}

bool isAbout(double value, double about, double threshold) {
	return (value > about - threshold) && (value < about + threshold);
}

double freqToRad(double freq) {
	return 2.0 * 3.1415926535 * freq / 441.0;
}
