#ifndef __MKBD_ARRAY_VIEW_HPP__
#define __MKBD_ARRAY_VIEW_HPP__

#include <cstdint>
#include <cmath>

template<class T>
class ArrayView {
	const T* mPtr = 0;
	int mStart = 0;
	int mEnd = 0;

public:
	typedef const T* iterator;

	ArrayView(void) {};
	ArrayView(const T* ptr, int start, int end)
	: mPtr(ptr), mStart(start), mEnd(end) {};
	ArrayView(const T* ptr, int length)
	: ArrayView(ptr, 0, length) {};

	int length(void) { return mEnd - mStart; };

	const T* getData(void) { return &mPtr[mStart]; };

	const T* getRawData(void) { return mPtr; };

	const T* begin(void) { return &mPtr[mStart]; };
	const T* end(void) { return &mPtr[mEnd]; };

	const T& get(int idx) {
		return mPtr[mStart + idx];
	};

	const T& operator[](int idx) { return get(idx); };
};

#endif