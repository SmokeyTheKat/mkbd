#include <mkbd/gui/graphics/input.hpp>

#include <string>
#include <iostream>

InputGraphic::InputGraphic(Layout layout, std::string text, std::function<void(void)> callback, InputType inputType)
: Graphic(layout), mCallback(callback), mInputType(inputType),
  mText(layout, text, RESOURCE_DIR "/fonts/FreeSans.ttf", 20, mFgColor) {};

void InputGraphic::init(void) {
	mText.setWindow(mWindow);
	mText.setAlign(FC_ALIGN_LEFT);
	mWindow->setGraphicsSize(&mText);
	setTextLayout();
}

void InputGraphic::setTextLayout(void) {
	mText.setX(mX - mWidth / 2 + 5);
	mText.setY(mY);
	mText.setWidth(mWidth);
	mText.setHeight(mHeight);
}

void InputGraphic::onResize(int width, int height) {
	mWindow->setGraphicsSize(&mText);
	setTextLayout();
}

void InputGraphic::onKeyDown(int key) {
	if (!isFocused()) return;

	std::string text = mText.getText();

	if (key == SDLK_BACKSPACE) {
		if (text.length() > 0)
			text.pop_back();
	} else if (key < 128) {
		text += (char)key;
	}

	mText.setText(text);
}

void InputGraphic::draw(void) {
	setColor1(RGB_ARGS(mBgColor));
	setColor2(RGB_ARGS(mFgColor));
	drawRectangleWithOutline(0, 0, mWidth, mHeight);
	mText.draw();
}
