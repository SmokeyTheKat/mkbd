#ifndef __MKBD_UTILS_HPP__
#define __MKBD_UTILS_HPP__

#include <mkbd/color.hpp>

#include <string>

typedef unsigned char byte;

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