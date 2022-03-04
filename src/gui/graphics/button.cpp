#include <mkbd/gui/graphics/button.hpp>

#include <mkbd/utils.hpp>

#include <string>

ButtonGraphic::ButtonGraphic(int x, int y, int width, int height, std::string text, std::function<void(void)> callback)
: Graphic(x, y, width, height), mCallback(callback),
  mText(x, y, width, height, text, "fonts/FreeSans.ttf", 20, mFgColor) {};

ButtonGraphic::ButtonGraphic(int x, int y, int width, int height, std::string text, std::function<void(void)> callback, Color bgColor, Color fgColor)
: Graphic(x, y, width, height), mCallback(callback),
  mBgColor(bgColor), mFgColor(fgColor),
  mText(x, y, width, height, text, "fonts/FreeSans.ttf", 20, fgColor) {};

void ButtonGraphic::onClick(int button, int x, int y) {
	mCallback();
}

void ButtonGraphic::init(void) {
	mText.setWindow(mWindow);
}

void ButtonGraphic::draw(void) {
	setColor(RGB_ARGS(mBgColor));
	fillRectangle(0, 0, mWidth, mHeight);
	setColor(RGB_ARGS(mFgColor));
	drawRectangle(0, 0, mWidth, mHeight);
	mText.draw();
}