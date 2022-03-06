#include <mkbd/gui/graphics/rectangle.hpp>

#include <mkbd/utils.hpp>

#include <iostream>

RectangleGraphic::RectangleGraphic(Layout layout, Color color)
: Graphic(layout), mColor(color) {};

void RectangleGraphic::draw(void) {
	setColor(RGB_ARGS(mColor));
	fillRectangle(0, 0, mWidth, mHeight);
	setColor(0, 0, 0);
}
