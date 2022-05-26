#include <mkbd/gui/components/input.hpp>

#include <string>
#include <iostream>

InputComponent::InputComponent(Layout layout, std::string text, InputType inputType)
: Component(layout), mInputType(inputType) {
	setCursor(SDL_SYSTEM_CURSOR_IBEAM);
	mText = new TextComponent(
		layout.resetPosition(),
		text,
		RESOURCE_DIR "/fonts/FreeSans.ttf",
		20,
		mFgColor
	);
	addChild(mText);
};

void InputComponent::init(void) {
}

void InputComponent::setText(std::string text) {
	mText->setText(text);
	mCursor = text.length();
}

void InputComponent::setAlign(FC_AlignEnum align) {
	mText->setAlign(align);
}

void InputComponent::setTextLayout(void) {
}

void InputComponent::onResize(int width, int height) {
	mWindow->setComponentsSize(mText);
	setTextLayout();
}

bool InputComponent::keyIsPrintable(int key) {
	return key >= ' ' && key <= '~';
}

bool InputComponent::stringFitsRestrictions(const std::string& str) {
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

bool InputComponent::keyFitsRestrictions(int key) {
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

void InputComponent::onKeyDown(int key) {
	if (!isFocused()) return;

	if (!keyFitsRestrictions(key)) return;

	std::string text = mText->getText();

	switch (key) {
		case SDLK_BACKSPACE: {
			if (text.length() > 0)
				text.pop_back();
			mCursor--;
		} break;
		case SDLK_RETURN: {
			emit<std::string>("Submit", text);
		} break;
		default: {
			if (!keyIsPrintable(key)) break;

			if (!stringFitsRestrictions(text + (char)key))
				return;

			text += (char)key;
			mCursor++;
		} break;
	}

	mText->setText(text);

	emit<std::string>("Change", text);
}

void InputComponent::draw(void) {
	setColor1(RGB_ARGS(mBgColor));

	if (isFocused())
		setColor2(RGB_ARGS(mFgColor.invert()));
	else
		setColor2(RGB_ARGS(mFgColor));

	drawRectangleWithOutline(0, 0, mWidth, mHeight);

	std::string str = mText->getText();
	mCursor = str.length();
	if (isFocused() && time(0) % 2 == 0) {
		std::string newstr = str;
		newstr.insert(mCursor, "|");
		mText->setText(newstr);
	}
	mText->draw();

	if (isFocused())
		mText->setText(str);
}
