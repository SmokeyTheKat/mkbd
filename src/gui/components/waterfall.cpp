#include <mkbd/gui/components/waterfall.hpp>

#include <mkbd/midi/recorder.hpp>
#include <mkbd/midi/keyboard.hpp>
#include <mkbd/music.hpp>
#include <mkbd/math.hpp>
#include <mkbd/settings.hpp>

#include <iostream>

WaterfallComponent::WaterfallComponent(Layout layout, MidiRecorder* recorder)
: Component(layout), mRcdr(recorder) {
	calculateSizes();
	mRcdr->on("Event", asFunction<MidiEvent>([this](MidiEvent e) {
		mEvents.push_back(e);
	}));
};

void WaterfallComponent::init(void) {
	calculateSizes();
}

void WaterfallComponent::onResize(int width, int height) {
	calculateSizes();
}

void WaterfallComponent::calculateSizes(void) {
	mKeyWidth = mWidth / mKeyCount;
	mKeyHeight = mHeight;
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

double WaterfallComponent::getKeyPressLength(std::vector<MidiEvent>::iterator it) {
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

int WaterfallComponent::getKeyWidth(int key) {
	std::string noteName = Music::getNoteName(key);
	if (noteName.length() == 2) {
		return mBlackKeyWidth;
	}
	return mKeyWidth;
}

void WaterfallComponent::drawLines(void) {
	double time = mRcdr->getTime();
	double beatLength = 60.0 / mRcdr->getBpm();
	double barLength = beatLength * 4.0;


	setColor(125, 125, 125);
	for (int i = 0; i < 4; i++) {
		double y = std::fmod(time - i * beatLength, barLength);
		y = rmap(y, 0, barLength, mHeight, 0);
		drawLine(0, y, mWidth, y);
	}

	setColor(255, 255, 255);
	double y = std::fmod(time, barLength);
	y = rmap(y, 0, barLength, mHeight, 0);
	drawLine(0, y, mWidth, y);

	setColor(125, 125, 125);
	for (int i = 24; i < 109; i += 12) {
		int x = mKeyPositions[i];
		drawLine(x, 0, x, mHeight);
	}

	setColor(80, 80, 80);
	for (int i = 29; i < 109; i += 12) {
		int x = mKeyPositions[i];
		drawLine(x, 0, x, mHeight);
	}
}

void WaterfallComponent::draw(void) {
	int scale = 240;
	setColor(35, 35, 35);
	fillRectangle(0, 0, mWidth, mHeight);

	std::vector<std::vector<MidiEvent>::iterator> toDelete;

	drawLines();

	double beatLength = 60.0 / mRcdr->getBpm();
	double barLength = beatLength * 4.0;

	for (auto it = mEvents.begin(); it != mEvents.end(); it++) {
		if (it->getType() == MidiEvent::NoteOn) {
			double length = getKeyPressLength(it);

			int height = rmap(length, 0, barLength, 0, mHeight);
			int y = rmap(mRcdr->getTime() - it->time, 0, barLength, mHeight, 0);
			int width = getKeyWidth((*it)[1]);

//            if (y - height > 1 && height < 30) height = 10;

			if (y + height < 0) {
				toDelete.push_back(it);
				continue;
			}

			if (y < 0) {
				height += y;
				y = 0;
			}
			if (getKeyWidth((*it)[1]) < getKeyWidth((*it)[1] + 1))
				setColor1(RGB_ARGS(gConfig.blackKeyDownColor));
			else
				setColor1(RGB_ARGS(gConfig.whiteKeyDownColor));
			setColor2(0, 0, 0);
			fillRectangle(mKeyPositions[(*it)[1]], y, width-2, height);
		}
	}

	for (auto it : toDelete) {
		mEvents.erase(it);
	}
}
