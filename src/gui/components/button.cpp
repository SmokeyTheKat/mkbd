#include <mkbd/gui/components/button.hpp>

#include <mkbd/utils.hpp>
#include <mkbd/eventemitter.hpp>

#include <string>
#include <iostream>

ButtonComponent::ButtonComponent(Layout layout, std::string text, std::function<void(void)> callback)
: Component(layout), mCallback(callback),
  mText(layout, text, RESOURCE_DIR "/fonts/FreeSans.ttf", 20, mFgColor) {};

ButtonComponent::ButtonComponent(Layout layout, std::string text, std::function<void(void)> callback, Color bgColor, Color fgColor)
: Component(layout), mCallback(callback),
  mBgColor(bgColor), mFgColor(fgColor),
  mText(layout, text, RESOURCE_DIR "/fonts/FreeSans.ttf", 20, fgColor) {};

void ButtonComponent::onClick(int button, int x, int y) {
	mCallback();
}

void ButtonComponent::onlick(int button, int x, int y) {
	mCallback();
}

void ButtonComponent::onResize(int width, int height) {
	mWindow->setComponentsSize(&mText);
}

void ButtonComponent::init(void) {
	mText.setWindow(mWindow);
	mText.setAlign(FC_ALIGN_CENTER);
	mWindow->setComponentsSize(&mText);
}


void ButtonComponent::draw(void) {
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