#include <mkbd/gui/graphics/waterfall.hpp>

#include <mkbd/midi/recorder.hpp>
#include <mkbd/midi/keyboard.hpp>
#include <mkbd/music.hpp>

#include <iostream>

WaterfallGraphic::WaterfallGraphic(Layout layout, MidiRecorder* recorder)
: Graphic(layout), mRcdr(recorder) {
	calculateSizes();
	mRcdr->on("Event", asFunction<MidiRecorder*, MidiEvent>([this](MidiRecorder* rcdr, MidiEvent e) {
		mEvents.push_back(e);
	}));
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

double WaterfallGraphic::getKeyPressLength(std::vector<MidiEvent>::iterator it) {
	MidiEvent startEvent = *it;
	for (; it != mEvents.end(); it++) {
		MidiEvent event = *it;
		if (event.getType() == MidiEvent::NoteOff && startEvent[1] == event[1]) {
			return event.time - startEvent.time;
			break;
		}
	}
	return mRcdr->getTime() - startEvent.time;
}

int WaterfallGraphic::getKeyWidth(int key) {
	std::string noteName = Music::getNoteName(key);
	if (noteName.length() == 2) {
		return mBlackKeyWidth;
	}
	return mKeyWidth;
}

void WaterfallGraphic::draw(void) {
	int scale = 240;
	setColor(48, 48, 48);
	fillRectangle(mX, mY, mWidth, mHeight);

	std::vector<std::vector<MidiEvent>::iterator> toDelete;

	for (auto it = mEvents.begin(); it != mEvents.end(); it++) {
		if (it->getType() == MidiEvent::NoteOn) {
			double length = getKeyPressLength(it);

			int height = length * scale;
			int y = mHeight - (mRcdr->getTime() * scale - it->time * scale);
			int width = getKeyWidth((*it)[1]);

			if (y - height > 1 && height < 30) height = 20;

			if (y + height < 0) {
				toDelete.push_back(it);
				continue;
			}

			setColor1(RGB_ARGS(mWhiteKeyColor));
			setColor2(0, 0, 0);
			drawRectangleWithOutline(mKeyPositions[(*it)[1]], y, width, height);
		}
	}

	for (auto it : toDelete) {
		mEvents.erase(it);
	}
}
