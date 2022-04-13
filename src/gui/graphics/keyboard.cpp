#include <mkbd/gui/graphics/keyboard.hpp>
#include <mkbd/midi/recorder.hpp>

#include <iostream>

using namespace std::placeholders;

KeyboardGraphic::KeyboardGraphic(Layout layout, MidiRecorder* rcdr)
: Graphic(layout), mRcdr(rcdr) {
	calculateSizes();
	rcdr->on("NoteOn", asFunction<byte, byte>(std::bind(&KeyboardGraphic::onNoteOn, this, _1, _2)));
	rcdr->on("NoteOff", asFunction<byte>(std::bind(&KeyboardGraphic::onNoteOff, this, _1)));
};

void KeyboardGraphic::onNoteOn(byte note, byte velocity) {
	keys[note - 12] = 1;
}

void KeyboardGraphic::onNoteOff(byte note) {
	keys[note - 12] = 0;
}

int KeyboardGraphic::getKeyAtPos(int x, int y) {
	for (int note = 0; note < mKeyPositions.size(); note++) {
		if (y > mBlackKeyHeight) break;

		int vw = mKeyWidth;
		if (!Music::isNoteFlat(note)) {
			vw = mBlackKeyWidth;
			int v = mKeyPositions[note];
			if (x >= v && x < v + vw) {
				return note;
			}
		}
	}
	for (int note = 0; note < mKeyPositions.size(); note++) {
		int vw = mKeyWidth;
		if (!Music::isNoteFlat(note)) {
			continue;
		}
		int v = mKeyPositions[note];
		if (x >= v && x < v + vw) {
			return note;
		}
	}
	return -1;
}

void KeyboardGraphic::onClick(int button, int x, int y) {
	mVKey = getKeyAtPos(x, y);
	mRcdr->sendEvent(MidiEvent({MidiEvent::NoteOn, mVKey, 50}));
}

void KeyboardGraphic::onDrag(int x, int y) {
	int key = getKeyAtPos(x, y);
	if (mVKey != key) {
		if (mVKey != -1)
			mRcdr->sendEvent(MidiEvent({MidiEvent::NoteOff, mVKey, 0}));

		onClick(0, x, y);
	}
}

void KeyboardGraphic::onMouseUp(int button, int x, int y) {
	mRcdr->sendEvent(MidiEvent({MidiEvent::NoteOff, mVKey, 0}));
	mVKey = -1;
}

void KeyboardGraphic::onLeave(int x, int y) {
	mRcdr->sendEvent(MidiEvent({MidiEvent::NoteOff, mVKey, 0}));
	mVKey = -1;
}

void KeyboardGraphic::onResize(int width, int height) {
	calculateSizes();
}

void KeyboardGraphic::init(void) {
	calculateSizes();
}

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

void KeyboardGraphic::calculateSizes(void) {
	mKeyWidth = mWidth / mKeyCount;
	mKeyHeight = mHeight;
	mBlackKeyWidth = (mKeyWidth * 3) / 5;
	mBlackKeyHeight = (mKeyHeight * 5) / 8;

	mKeyPositions.resize(127);

	for (int i = 0, key = 9 + 12; i < mKeyCount + 0; i++) {
		mKeyPositions[key] = i * mKeyWidth;

		key++;

		if ((i+5) % 7 == 0) {
			int offset = (5 * mBlackKeyWidth) / 8;
			mKeyPositions[key] = (i + 1) * mKeyWidth - offset;
			key++;
		} else if ((i+5) % 7 == 1) {
			int offset = (3 * mBlackKeyWidth) / 8;
			mKeyPositions[key] = (i + 1) * mKeyWidth - offset;
			key++;
		} else if ((i+5) % 7 == 3) {
			int offset = (5 * mBlackKeyWidth) / 8;
			mKeyPositions[key] = (i + 1) * mKeyWidth - offset;
			key++;
		} else if ((i+5) % 7 == 4) {
			int offset = mBlackKeyWidth / 2;
			mKeyPositions[key] = (i + 1) * mKeyWidth - offset;
			key++;
		} else if ((i+5) % 7 == 5) {
			int offset = (3 * mBlackKeyWidth) / 8;
			mKeyPositions[key] = (i + 1) * mKeyWidth - offset;
			key++;
		}
	}
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
