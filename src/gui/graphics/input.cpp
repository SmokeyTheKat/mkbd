#include <mkbd/gui/graphics/input.hpp>

#include <string>
#include <iostream>

InputGraphic::InputGraphic(Layout layout, std::string text, InputType inputType)
: Graphic(layout), mInputType(inputType),
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

bool InputGraphic::keyIsPrintable(int key) {
	return key >= ' ' && key <= '~';
}

bool InputGraphic::stringFitsRestrictions(const std::string& str) {
	if (str.length() == 0)
		return true;

	switch (mInputType) {
		case InputType::Number: {
			intmax_t value = std::stoi(str);
			if (value > mMaxValue)
				return false;
		} break;
	};
	return true;
}

bool InputGraphic::keyFitsRestrictions(int key) {
	if (!keyIsPrintable(key))
		return true;

	switch (mInputType) {
		case InputType::Text: {
			return true;
		} break;
		case InputType::Number: {
			if (key >= '0' && key <= '9')
				return true;
			else
				return false;
		} break;
	};
	return false;
}

void InputGraphic::onKeyDown(int key) {
	if (!isFocused()) return;

	if (!keyFitsRestrictions(key)) return;

	std::string text = mText.getText();

	switch (key) {
		case SDLK_BACKSPACE: {
			if (text.length() > 0)
				text.pop_back();
		} break;
		case SDLK_RETURN: {
		} break;
		default: {
			if (!keyIsPrintable(key)) break;

			if (!stringFitsRestrictions(text + (char)key))
				return;

			text += (char)key;
		} break;
	}

	mText.setText(text);

	emit<std::string>("change", text);
}

void InputGraphic::draw(void) {
	setColor1(RGB_ARGS(mBgColor));
	setColor2(RGB_ARGS(mFgColor));
	drawRectangleWithOutline(0, 0, mWidth, mHeight);
	mText.draw();
}
