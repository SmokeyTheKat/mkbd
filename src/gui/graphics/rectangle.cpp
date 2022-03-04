#include <mkbd/gui/graphics/rectangle.hpp>

#include <mkbd/utils.hpp>

#include <iostream>

RectangleGraphic::RectangleGraphic(int x, int y, int width, int height, Color color)
: Graphic(x, y, width, height), mColor(color) {};

void RectangleGraphic::draw(void) {
	setColor(RGB_ARGS(mColor));
	std::cout << "h: " << mHeight << "\n";
	fillRectangle(0, 0, mWidth, mHeight);
	setColor(0, 0, 0);
}
