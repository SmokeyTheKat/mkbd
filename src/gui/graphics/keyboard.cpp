#include <mkbd/gui/graphics/keyboard.hpp>

#include <iostream>

KeyboardGraphic::KeyboardGraphic(int x, int y, int width, int height)
: Graphic(x, y, width, height), mKeyWidth(mWidth / mKeyCount), mKeyHeight(mHeight),
  mBlackKeyWidth((mKeyWidth * 3) / 5), mBlackKeyHeight((mKeyHeight * 5) / 8) {};

Color KeyboardGraphic::getWhiteKeyColor(int key) {
	if (keys[key]) {
		return mKeyDownColor;
	}
	return mWhiteKeyColor;
}

Color KeyboardGraphic::getBlackKeyColor(int key) {
	if (keys[key]) {
		return mKeyDownColor;
	}
	return mBlackKeyColor;
}

void KeyboardGraphic::drawKeyboardBlackKey(int key, int i, int offset) {
	Color color = getBlackKeyColor(key);
	setColor1(RGB_ARGS(color));
	drawRectangleWithOutline((i + 1) * mKeyWidth - offset, 0, mBlackKeyWidth, mBlackKeyHeight);
}

void KeyboardGraphic::drawKeyboardBlackKeyLeft(int key, int i) {
	int offset = (5 * mBlackKeyWidth) / 8;
	drawKeyboardBlackKey(key, i, offset);
}

void KeyboardGraphic::drawKeyboardBlackKeyRight(int key, int i) {
	int offset = (3 * mBlackKeyWidth) / 8;
	drawKeyboardBlackKey(key, i, offset);
}

void KeyboardGraphic::drawKeyboardBlackKeyMiddle(int key, int i) {
	int offset = mBlackKeyWidth / 2;
	drawKeyboardBlackKey(key, i, offset);
}

void KeyboardGraphic::draw(void) {
	setColor1(RGB_ARGS(mWhiteKeyColor));
	setColor2(RGB_ARGS(mBlackKeyColor));

	for (int i = 0, key = 9; i < mKeyCount + 0; i++) {
		Color color = getWhiteKeyColor(key);
		setColor1(RGB_ARGS(color));
		drawRectangleWithOutline(i * mKeyWidth, 0, mKeyWidth, mKeyHeight);

		key++;

		if ((i+5) % 7 == 0) key++;
		else if ((i+5) % 7 == 1) key++;
		else if ((i+5) % 7 == 3) key++;
		else if ((i+5) % 7 == 4) key++;
		else if ((i+5) % 7 == 5) key++;
	}

	for (int i = 0, key = 9; i < mKeyCount + 0; i++) {
		Color color = getWhiteKeyColor(key);
		setColor1(RGB_ARGS(color));
		key++;
		if ((i+5) % 7 == 0)
			drawKeyboardBlackKeyLeft(key++, i);
		else if ((i+5) % 7 == 1)
			drawKeyboardBlackKeyRight(key++, i);
		else if ((i+5) % 7 == 3)
			drawKeyboardBlackKeyLeft(key++, i);
		else if ((i+5) % 7 == 4)
			drawKeyboardBlackKeyMiddle(key++, i);
		else if ((i+5) % 7 == 5)
			drawKeyboardBlackKeyRight(key++, i);
	}
}
