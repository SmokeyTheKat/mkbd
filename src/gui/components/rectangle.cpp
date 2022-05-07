#include <mkbd/gui/components/rectangle.hpp>

#include <mkbd/utils.hpp>

#include <iostream>

RectangleComponent::RectangleComponent(Layout layout, Color color, Color outline)
: Component(layout), mColor(color), mOutline(outline) {};

void RectangleComponent::draw(void) {
	setColor(RGB_ARGS(mColor));
	if (mOutline.isColor())
		setColor2(RGB_ARGS(mOutline));
	else setColor2(RGB_ARGS(mColor));
	drawRectangleWithOutline(0, 0, mWidth, mHeight);
	setColor(0, 0, 0);
}
