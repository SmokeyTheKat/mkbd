#ifndef __MKBD_ARRAY_VIEW_HPP__
#define __MKBD_ARRAY_VIEW_HPP__

#include <cstdint>
#include <cmath>

template<class T>
class ArrayView {
	const uint8_t* mPtr = 0;
	int mStart = 0;
	int mEnd = 0;

public:
	typedef const T* iterator;

	ArrayView(void) {};
	ArrayView(const T* ptr, int start, int end)
	: mPtr((const uint8_t*)ptr), mStart(start * sizeof(T)), mEnd(end * sizeof(T)) {};
	ArrayView(const T* ptr, int length)
	: ArrayView(ptr, 0, length) {};

	template<class R = T>
	int length(void) { return (mEnd - mStart) / sizeof(R); };

	double getNumberAt(int bitSize, int idx) {
		int byteSize = bitSize / 8;

		if (byteSize == sizeof(T)) {
			return get(idx);
		}

		double value = 0;
		for (int i = 0; i < byteSize; i++) {
			value += (double)mPtr[mStart + idx * byteSize + i] * std::pow(2, i * 8);
		}

		if (value > std::pow(2, bitSize-1)) {
			value = value - (2 * std::pow(2, bitSize-1));
		}

		return value;
	};

	template<class R = T>
	const R* getData(void) { return (R*)&mPtr[mStart]; };

	template<class R = T>
	const R* getRawData(void) { return (R*)mPtr; };

	const T* begin(void) { return (T*)&mPtr[mStart]; };
	const T* end(void) { return (T*)&mPtr[mEnd]; };

	template<class R = T>
	const T& get(int idx) {
		return *(R*)&mPtr[mStart + idx * sizeof(R)];
	};

	const T& operator[](int idx) { return get(idx); };
};

#endif