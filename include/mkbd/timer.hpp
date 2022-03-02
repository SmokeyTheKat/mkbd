#ifndef __MKBD_TIMER_HPP__
#define __MKBD_TIMER_HPP__

#include <chrono>

class Timer {
	std::chrono::steady_clock::time_point mStart;
	std::chrono::steady_clock::time_point mLap;
public:
	void start(void);
	double now(void);
	double lap(void);
	double getLap(void);
	double stop(void);
};

#endif