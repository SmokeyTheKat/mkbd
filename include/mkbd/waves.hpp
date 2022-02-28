#ifndef __MKBD_WAVES_HPP__
#define __MKBD_WAVES_HPP__

namespace Waves {
	double sine(double t, double freq);
	double square(double t, double freq);
	double pulse(double t, double freq, double percent);
	double piano(double t, double freq);
	double piano2(double t, double freq);
	double noise(void);
};

#endif