#ifndef __MKBD_WAVES_HPP__
#define __MKBD_WAVES_HPP__

namespace Waves {
	double sine(double t, double freq);
	double square(double t, double freq);
	double triangle(double t, double freq);
	double hexagon(double t, double freq);
	double sawtooth(double t, double freq);
	double multi(double t, double freq, double a, double b, double c);
	double pulse(double t, double freq, double percent);
	double noise(void);
	double piano(double t, double freq);
};

#endif