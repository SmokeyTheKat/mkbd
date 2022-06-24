#ifndef __MKBD_UTILS_HPP__
#define __MKBD_UTILS_HPP__

#include <mkbd/color.hpp>

#include <string>
#include <vector>
#include <string_view>

typedef unsigned char byte;

#define MAX(a, b) ((a > b) ? (a) : (b))
#define MIN(a, b) ((a < b) ? (a) : (b))

#define __RSEQ_N() \
	63,62,61,60,                   \
	59,58,57,56,55,54,53,52,51,50, \
	49,48,47,46,45,44,43,42,41,40, \
	39,38,37,36,35,34,33,32,31,30, \
	29,28,27,26,25,24,23,22,21,20, \
	19,18,17,16,15,14,13,12,11,10, \
	9,8,7,6,5,4,3,2,1,0
#define __NARG__(...)  __NARG_I_(__VA_ARGS__,__RSEQ_N())
#define __NARG_I_(...) __ARG_N(__VA_ARGS__)
#define __ARG_N( \
		_1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
		_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
		_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
		_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
		_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
		_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
		_61,_62,_63,N,...) N

#define _VFUNC_(name, n) name##n
#define _VFUNC(name, n) _VFUNC_(name, n)
#define VFUNC(func, ...) _VFUNC(func, __NARG__(__VA_ARGS__)) (__VA_ARGS__)

enum class NumberFormat {
	BigEndiain,
	LittleEndiain,
};

namespace Utils {
	int getVarIntLength(const std::vector<byte>& data);
	std::string runCommand(std::string_view cmd);
	bool stringIsNumber(std::string str);
};


class File {
	std::string mPath;
	std::vector<byte> mData;
	int mPosition = 0;

public:
	File(std::string path)
	: mPath(path) {};

	void open(void);

	int length(void) { return mData.size(); };

	byte& front(void) { return mData[0]; };

	int getPosition(void) { return mPosition; };

	byte& operator[](int idx) {
		return mData[idx];
	};

	std::vector<byte> pop(int count) {
		std::vector<byte> slice(mData.begin(), mData.begin() + count);
		mData.erase(mData.begin(), mData.begin() + count);
		mPosition += count;
		return slice;
	};

	std::string popString(int count) {
		std::string slice(mData.begin(), mData.begin() + count);
		pop(count);
		return slice;
	};

	template<class T = uintmax_t>
	T popNumber(int count, NumberFormat format = NumberFormat::BigEndiain) {
		T value = 0;
		if (format == NumberFormat::LittleEndiain) {
			value = *(T*)&mData[0];
		}
		else {
			for (int i = count - 1, j = 0; i >= 0; i--, j += 8)
				value |= (int)mData[i] << j;
		}
		pop(count);
		return value;
	};

	template<class T = uintmax_t>
	T popVarInt(NumberFormat format = NumberFormat::BigEndiain) {
		int length = Utils::getVarIntLength(mData);
		T result = 0;
		for (int i = 0; i < length; i++) {
			int b = mData[i];
			if (b & 0x80) {
				result += (b & 0x7f);
				result <<= 7;
			} else {
				result += b;
			}
		};

		pop(length);
		return result;
	};
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