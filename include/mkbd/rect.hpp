#ifndef __MKBD_RECT_HPP__
#define __MKBD_RECT_HPP__

struct Rect {
	int x;
	int y;
	int width;
	int height;

	Rect(void)
	: x(0), y(0), width(0), height(0) {};
	Rect(int x, int y, int width, int height)
	: x(x), y(y), width(width), height(height) {};

	bool isPointIntersecting(int x, int y);
};

#endif