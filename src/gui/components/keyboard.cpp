#include <mkbd/gui/components/keyboard.hpp>

#include <mkbd/midi/recorder.hpp>
#include <mkbd/settings.hpp>

#include <iostream>

using namespace std::placeholders;

KeyboardComponent::KeyboardComponent(Layout layout, MidiRecorder* rcdr)
: Component(layout), mRcdr(rcdr) {
	calculateSizes();
	rcdr->on("NoteOn", asFunction<byte, byte>(std::bind(&KeyboardComponent::onNoteOn, this, _1, _2)));
	rcdr->on("NoteOff", asFunction<byte>(std::bind(&KeyboardComponent::onNoteOff, this, _1)));
};

void KeyboardComponent::onNoteOn(byte note, byte velocity) {
	keys[note - 12] = 1;
}

void KeyboardComponent::onNoteOff(byte note) {
	keys[note - 12] = 0;
}

int KeyboardComponent::getKeyAtPos(int x, int y) {
	y -= mBorder;
	if (y < 0) return -1;
	for (int note = 0; note < mKeyPositions.size(); note++) {
		if (y >= mBlackKeyHeight) break;

		if (!Music::isNoteFlat(note)) {
			int v = mKeyPositions[note];
			if (x >= v && x < v + mBlackKeyWidth) {
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

void KeyboardComponent::onClick(int button, int x, int y) {
	mVKey = getKeyAtPos(x, y);
	mRcdr->sendEvent(MidiEvent({MidiEvent::NoteOn, mVKey, 50}));
}

void KeyboardComponent::onDrag(int x, int y) {
	int key = getKeyAtPos(x, y);
	if (mVKey != key) {
		if (mVKey != -1)
			mRcdr->sendEvent(MidiEvent({MidiEvent::NoteOff, mVKey, 0}));

		onClick(0, x, y);
	}
}

void KeyboardComponent::onMouseUp(int button, int x, int y) {
	mRcdr->sendEvent(MidiEvent({MidiEvent::NoteOff, mVKey, 0}));
	mVKey = -1;
}

void KeyboardComponent::onLeave(int x, int y) {
	mRcdr->sendEvent(MidiEvent({MidiEvent::NoteOff, mVKey, 0}));
	mVKey = -1;
}

void KeyboardComponent::onResize(int width, int height) {
	calculateSizes();
}

void KeyboardComponent::init(void) {
	calculateSizes();
}

Color KeyboardComponent::getWhiteKeyColor(int key) {
	if (gConfig.showKeyDownColor && keys[key]) {
		if (Music::isBlackKey(key))
			return gConfig.blackKeyDownColor;
		else return gConfig.whiteKeyDownColor;
	}
	return gConfig.whiteKeyColor;
}

Color KeyboardComponent::getBlackKeyColor(int key) {
	if (gConfig.showKeyDownColor && keys[key]) {
		if (Music::isBlackKey(key))
			return gConfig.blackKeyDownColor;
		else return gConfig.whiteKeyDownColor;
	}
	return gConfig.blackKeyColor;
}

void KeyboardComponent::drawKey3DPart(int key, Color color, int x, int width, int height) {
	color = color.darken(0.2);
	setColor1(RGB_ARGS(color));
	double angle = mViewAngle;
	if (keys[key]) {
		if (gConfig.showKeyPress)
			angle *= 0.5;
	}
	int shadowHeight = height * angle;
	drawRectangleWithOutline(
		x, mBorder + height - shadowHeight,
		width, shadowHeight
	);
}

void KeyboardComponent::drawKeyPushShadowLeft(int i, Color color, int offset) {
//    int pushDistanceWidth = mPushDistance / 7;
	int pushDistanceWidth = 0;
	int pushDistanceHeight = mPushDistance;

	setColor1(RGB_ARGS(color));

	int wid = ((i + 1) * mKeyWidth) - ((i + 1) * mKeyWidth - offset - pushDistanceWidth);
	fillRectangle((i + 1) * mKeyWidth - offset - pushDistanceWidth, mBorder - 1, pushDistanceWidth, mBlackKeyHeight);
	fillRectangle((i + 1) * mKeyWidth - offset - pushDistanceWidth, mBorder - 1 + mBlackKeyHeight, wid, pushDistanceHeight);
}

void KeyboardComponent::drawKeyPushShadowRight(int i, Color color, int offset) {
//    int pushDistanceWidth = mPushDistance / 7;
	int pushDistanceWidth = 0;
	int pushDistanceHeight = mPushDistance;

	setColor1(RGB_ARGS(color));

	int wid = ((i + 1) * mKeyWidth - offset + mBlackKeyWidth) - ((i + 1) * mKeyWidth) + pushDistanceWidth;
	fillRectangle((i + 1) * mKeyWidth - offset + mBlackKeyWidth, mBorder - 1, pushDistanceWidth, mBlackKeyHeight);
	fillRectangle((i + 1) * mKeyWidth, mBorder - 1 + mBlackKeyHeight, wid, pushDistanceHeight);
}

void KeyboardComponent::drawKeyboardWhiteKey(int key, int i) {
	Color color = getWhiteKeyColor(key);
	setColor1(RGB_ARGS(color));
	drawRectangleWithOutline(i * mKeyWidth, mBorder, mKeyWidth, mKeyHeight);

	if (gConfig.showKeys3D) {
		drawKey3DPart(key, color, i * mKeyWidth, mKeyWidth, mKeyHeight);
	}
}

void KeyboardComponent::drawKeyboardBlackKey(int key, int i, int offset) {
	Color color = getBlackKeyColor(key);
	setColor1(RGB_ARGS(color));
	drawRectangleWithOutline((i + 1) * mKeyWidth - offset, mBorder - 1, mBlackKeyWidth, mBlackKeyHeight);

	if (gConfig.showKeys3D) {
		drawKey3DPart(key, color.darken(0.2), (i + 1) * mKeyWidth - offset, mBlackKeyWidth, mBlackKeyHeight);

		color = color.darken(0.2);

//        if (keys[key]) {
			color = color.darken(0.2);
//        }

		if (keys[key-1]) {
			drawKeyPushShadowLeft(i, color, offset);
		}
		if (keys[key+1]) {
			drawKeyPushShadowRight(i, color, offset);
		}
	}
}

void KeyboardComponent::drawKeyboardBlackKeyLeft(int key, int i) {
	int offset = (5 * mBlackKeyWidth) / 8;
	drawKeyboardBlackKey(key, i, offset);
}

void KeyboardComponent::drawKeyboardBlackKeyRight(int key, int i) {
	int offset = (3 * mBlackKeyWidth) / 8;
	drawKeyboardBlackKey(key, i, offset);
}

void KeyboardComponent::drawKeyboardBlackKeyMiddle(int key, int i) {
	int offset = mBlackKeyWidth / 2;
	drawKeyboardBlackKey(key, i, offset);
}

void KeyboardComponent::calculateSizes(void) {
	mKeyWidth = mWidth / mKeyCount;
	mKeyHeight = mHeight - mBorder;
	mBlackKeyWidth = (mKeyWidth * 7) / 10;
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

void KeyboardComponent::draw(void) {
	int fabricHeight = 2;
	setColor1(RGB_ARGS(Color(80)));
	fillRectangle(0, 0, mWidth, mBorder - fabricHeight);
	setColor1(RGB_ARGS(Colors::Red));
	fillRectangle(0, mBorder - fabricHeight, mWidth, fabricHeight);

	setColor1(RGB_ARGS(gConfig.whiteKeyColor));
	setColor2(RGB_ARGS(gConfig.blackKeyColor));

	for (int i = 0, key = 9; i < mKeyCount + 10; i++) {
		drawKeyboardWhiteKey(key, i);

		key++;

		if ((i+5) % 7 == 0) key++;
		else if ((i+5) % 7 == 1) key++;
		else if ((i+5) % 7 == 3) key++;
		else if ((i+5) % 7 == 4) key++;
		else if ((i+5) % 7 == 5) key++;
	}

	for (int i = 0, key = 9; i < mKeyCount + 10; i++) {
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
