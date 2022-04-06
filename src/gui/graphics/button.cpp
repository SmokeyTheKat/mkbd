#include <mkbd/gui/graphics/button.hpp>

#include <mkbd/utils.hpp>
#include <mkbd/eventemitter.hpp>

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

void ButtonGraphic::onlick(int button, int x, int y) {
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
	Color bg = mBgColor;
	if (isHovered()) {
		if (bg.r < 220) bg.r *= 1.2;
		else bg.r *= 0.8;

		if (bg.g < 220) bg.g *= 1.2;
		else bg.g *= 0.8;

		if (bg.b < 220) bg.b *= 1.2;
		else bg.b *= 0.8;
	}

	setColor(RGB_ARGS(bg));
	fillRectangle(0, 0, mWidth, mHeight);
	setColor(RGB_ARGS(mFgColor));
	drawRectangle(0, 0, mWidth, mHeight);
	mText.draw();
}