#include <mkbd/timer.hpp>

#include <iostream>

void Timer::start(void) {
	mStart = std::chrono::steady_clock::now();
	mLap = std::chrono::steady_clock::now();
}

double Timer::lap(void) {
	double lapTime = getLap();
	mLap = std::chrono::steady_clock::now();
	return lapTime;
}

double Timer::getLap(void) {
	std::chrono::steady_clock::time_point cur = std::chrono::steady_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(cur - mLap);
	return std::chrono::duration<double>(duration).count();
}

double Timer::now(void) {
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - mStart);
	return std::chrono::duration<double>(duration).count();
}

double Timer::stop(void) {
	return now();
}
