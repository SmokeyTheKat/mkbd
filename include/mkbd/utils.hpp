#ifndef __MKBD_UTILS_HPP__
#define __MKBD_UTILS_HPP__

#include <mkbd/color.hpp>

#include <string>
#include <string_view>

typedef unsigned char byte;

#define MAX(a, b) ((a > b) ? (a) : (b))
#define MIN(a, b) ((a < b) ? (a) : (b))

namespace Utils {
	std::string runCommand(std::string_view cmd);
	bool stringIsNumber(std::string str);
};

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

#endif