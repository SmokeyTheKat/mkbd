#ifndef __MKBD_UTILS_HPP__
#define __MKBD_UTILS_HPP__

#include <mkbd/color.hpp>

#include <string>

typedef unsigned char byte;

#define MAX(a, b) ((a > b) ? (a) : (b))
#define MIN(a, b) ((a < b) ? (a) : (b))

struct FlipFlop {
	bool state = false;

	void reset(void) { state = false; };

	operator bool(void) {
		if (!state) {
			state = true;
		}
		return true;
	};

	bool operator==(bool target) {
		if (target == true) {
			if (!state) {
				state = true;
				return true;
			}
			return false;
		}

		state = false;
		return false;
	};
};

inline bool stringIsNumber(std::string str) {
	if (str.length() == 0)
		return false;

	for (auto& c : str) {
		if (!std::isdigit(c))
			return false;
	}
	return true;
}

#endif