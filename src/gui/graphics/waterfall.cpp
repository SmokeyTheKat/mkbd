#include <mkbd/gui/graphics/waterfall.hpp>

#include <mkbd/recorder.hpp>
#include <mkbd/keyboard.hpp>
#include <mkbd/music.hpp>

#include <iostream>

WaterfallGraphic::WaterfallGraphic(Layout layout, KeyboardRecorder* recorder)
: Graphic(layout), mRcdr(recorder) {
	calculateSizes();
};

void WaterfallGraphic::init(void) {
	calculateSizes();
}

void WaterfallGraphic::onResize(int width, int height) {
	calculateSizes();
}

void WaterfallGraphic::calculateSizes(void) {
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

double WaterfallGraphic::getKeyPressLength(std::vector<KeyboardMessage>::iterator it) {
	KeyboardMessage startMsg = *it;
	for (; it != mMsgs.end(); it++) {
		KeyboardMessage msg = *it;
		if (msg.getType() == KBD_MSG_KEY_UP && startMsg[1] == msg[1]) {
			return msg.stamp - startMsg.stamp;
			break;
		}
	}
	return mRcdr->getTime() - startMsg.stamp;
}

int WaterfallGraphic::getKeyWidth(int key) {
	const char* keyName = getKeyNameFromKey(key);
	if (strlen(keyName) == 2) {
		return mBlackKeyWidth;
	}
	return mKeyWidth;
}

void WaterfallGraphic::draw(void) {
	int scale = 240;
	setColor(48, 48, 48);
	fillRectangle(mX, mY, mWidth, mHeight);
	mMsgs = mRcdr->getMessages();

	for (auto it = mMsgs.begin(); it != mMsgs.end(); it++) {
		if (it->getType() == KBD_MSG_KEY) {
			double length = getKeyPressLength(it);

			int height = length * scale;
			int y = mHeight - (mRcdr->getTime() * scale - it->stamp * scale);
			int width = getKeyWidth((*it)[1]);

			if (y - height > 1 && height < 30) height = 20;

			if (y + height < 0) {
				continue;
			}

			setColor1(RGB_ARGS(mWhiteKeyColor));
			setColor2(0, 0, 0);
			drawRectangleWithOutline(mKeyPositions[(*it)[1]], y, width, height);
		}
	}
}
