#include <mkbd/timer.hpp>

#include <iostream>

void Timer::start(void) {
	_start = std::chrono::steady_clock::now();
	_lap = std::chrono::steady_clock::now();
}

double Timer::lap(void) {
	double lapTime = getLap();
	_lap = std::chrono::steady_clock::now();
	return lapTime;
}

double Timer::getLap(void) {
	std::chrono::steady_clock::time_point cur = std::chrono::steady_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(cur - _lap);
	return std::chrono::duration<double>(duration).count();
}

double Timer::now(void) {
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start);
	return std::chrono::duration<double>(duration).count();
}

double Timer::stop(void) {
	return now();
}
