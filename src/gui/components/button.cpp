#include <mkbd/gui/components/button.hpp>

#include <mkbd/utils.hpp>
#include <mkbd/eventemitter.hpp>

#include <string>
#include <iostream>

ButtonComponent::ButtonComponent(Layout layout, std::string text, std::function<void(void)> callback, Color bgColor, Color fgColor)
: Component(layout), mCallback(callback), mBgColor(bgColor), mFgColor(fgColor) {
	setCursor(SDL_SYSTEM_CURSOR_HAND);

	mText = new TextComponent(
		layout.resetPosition(),
		text,
		RESOURCE_DIR "/fonts/FreeSans.ttf",
		20,
		mFgColor
	);
	mText->setAlign(FC_ALIGN_CENTER);
	addChild(mText);
};

void ButtonComponent::onClick(int button, int x, int y) {
	mCallback();
}

void ButtonComponent::onlick(int button, int x, int y) {
	mCallback();
}

void ButtonComponent::onResize(int width, int height) {
}

void ButtonComponent::init(void) {
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
}
