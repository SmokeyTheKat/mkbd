#include <mkbd/gui/graphics/rectangle.hpp>

#include <mkbd/utils.hpp>

#include <iostream>

RectangleGraphic::RectangleGraphic(Layout layout, Color color, Color outline)
: Graphic(layout), mColor(color), mOutline(outline) {};

void RectangleGraphic::draw(void) {
	setColor(RGB_ARGS(mColor));
	if (mOutline.isColor())
		setColor2(RGB_ARGS(mOutline));
	else setColor2(RGB_ARGS(mColor));
	drawRectangleWithOutline(0, 0, mWidth, mHeight);
	setColor(0, 0, 0);
}
