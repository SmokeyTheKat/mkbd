#include <mkbd/waves.hpp>

#include <mkbd/math.hpp>

#include <cmath>
#include <cstdlib>

double Waves::sine(double t, double freq) {
	return sin(freqToRad(freq) * t);
}

double _piano(double t) {
	double pi = 3.1415926535;
	return (-1.0 / 4.0) * sin(3.0 * pi * t) +
		(1.0 / 4.0) * sin(pi * t) -
		(std::sqrt(3) / 2.0) * cos(pi * t);
}

double Waves::piano(double t, double freq) {
	return _piano(freq * t);
}

double Waves::piano2(double t, double freq) {
	double pi = 3.1415926535;
	double r = freqToRad(freq);
	double y = 0;
	y += sin(1.0 * r * t) * std::exp(-0.0004 * r * t) / 1.0;
	y += sin(2.0 * r * t) * std::exp(-0.0004 * r * t) / 2.0;
	y += sin(3.0 * r * t) * std::exp(-0.0004 * r * t) / 4.0;
	y += sin(4.0 * r * t) * std::exp(-0.0004 * r * t) / 8.0;
	y += sin(5.0 * r * t) * std::exp(-0.0004 * r * t) / 16.0;
	y += sin(6.0 * r * t) * std::exp(-0.0004 * r * t) / 32.0;
	y += y * y * y;
	y *= 1.0 + 16.0 * t * std::exp(-6.0 * t);
	return y;
}

double Waves::square(double t, double freq) {
	double period = 1.0 / (freq);
	double offset = std::fmod(t, period);
	return offset > (0.5 * period) ? 1.0 : -1.0;
}

double Waves::pulse(double t, double freq, double percent) {
	percent /= 100.0;
	double period = 1.0 / (freq);
	double offset = std::fmod(t, period);
	return offset > (percent * period) ? 1.0 : -1.0;
}

double Waves::noise(void) {
	return (std::fmod(std::rand(), 3)) - 1.0;
}
