#include <mkbd/gui/graphics/button.hpp>

#include <mkbd/utils.hpp>

#include <string>
#include <iostream>

ButtonGraphic::ButtonGraphic(Layout layout, std::string text, std::function<void(void)> callback)
: Graphic(layout), mCallback(callback),
  mText(layout, text, RESOURCE_DIR "/fonts/FreeSans.ttf", 20, mFgColor) {};

ButtonGraphic::ButtonGraphic(Layout layout, std::string text, std::function<void(void)> callback, Color bgColor, Color fgColor)
: Graphic(layout), mCallback(callback),
  mBgColor(bgColor), mFgColor(fgColor),
  mText(layout, text, RESOURCE_DIR "/fonts/FreeSans.ttf", 20, fgColor) {};

void ButtonGraphic::onClick(int button, int x, int y) {
	mCallback();
}

void ButtonGraphic::onResize(int width, int height) {
	mWindow->setGraphicsSize(&mText);
}

void ButtonGraphic::init(void) {
	mText.setWindow(mWindow);
	mText.setAlign(FC_ALIGN_CENTER);
	mWindow->setGraphicsSize(&mText);
}

void ButtonGraphic::draw(void) {
	if (isHovered()) {
		setColor(mBgColor.r + 30, mBgColor.g + 30, mBgColor.b + 30);
	}
	else setColor(RGB_ARGS(mBgColor));
	fillRectangle(0, 0, mWidth, mHeight);
	setColor(RGB_ARGS(mFgColor));
	drawRectangle(0, 0, mWidth, mHeight);
	mText.draw();
}