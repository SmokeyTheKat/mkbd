#include <mkbd/waves.hpp>

#include <mkbd/math.hpp>

#include <cmath>
#include <cstdlib>

static double pi = 3.1415926535;

double Waves::sine(double t, double freq) {
	return sin(freqToRad(freq) * t);
}

double Waves::square(double t, double freq) {
	double period = 1.0 / (freq);
	double offset = std::fmod(t, period);
	return offset > (0.5 * period) ? 1.0 : -1.0;
}

double Waves::hexagon(double t, double freq) {
	double r = freqToRad(freq) * t;
	return 0.5 * asin(sin(r)) + 0.5 * asin(sin(r + pi / 2.0));
}

double Waves::triangle(double t, double freq) {
	return asin(sin(freqToRad(freq) * t));
}

double Waves::sawtooth(double t, double freq) {
	return atan(tan((freqToRad(freq) * t) / 2));
}

double Waves::multi(double t, double freq, double a, double b, double c) {
	return a * asin(sin(b * freqToRad(freq) * t)) + c * sin(freqToRad(freq) * t);
}

double Waves::pulse(double t, double freq, double percent) {
	percent /= 100.0;
	double period = 1.0 / (freq);
	double offset = std::fmod(t, period);
	return offset > (percent * period) ? 1.0 : -1.0;
}

double Waves::noise(void) {
	return std::fmod(std::rand(), 3.0) - 1.0;
}

double Waves::synth(double t, double freq) {
	double r = freqToRad(freq);
	return (std::pow(cos(t * r), 2) * sin(t * r)) / 0.324444;
}

double Waves::QU(double t, double freq) {
	double r = freqToRad(freq);
	double x = t * r;
	return sin(x) + sin(1.1 * x);
}

double Waves::piano(double t, double freq) {
//    double r = freqToRad(freq);
//    double x = -t * r;
//    double y = sin(x) + sin(2.0*x) + sin(2.5*x) + Waves::noise() * 0.001;
//    return y;

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
	return y + Waves::noise() * 0.01;
}
