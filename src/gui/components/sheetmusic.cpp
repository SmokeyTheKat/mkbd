#include <mkbd/gui/components/sheetmusic.hpp>

#include <mkbd/gui/window.hpp>
#include <mkbd/math.hpp>

SheetMusicComponent::SheetMusicComponent(Layout layout, MidiRecorder* rcdr)
: Component(layout), mRcdr(rcdr),
  mTrebleClefTexture(RESOURCE_DIR "/images/trebleclef.png"), mQuaterNoteTexture(RESOURCE_DIR "/images/quarternote.png"),
  mBassClefTexture(RESOURCE_DIR "/images/bassclef.png") {
	mRcdr->on("NoteOn", asFunction<byte, byte>([this](byte note, byte vel) {
		mNotes.push_back(Music::Note(note, vel, mRcdr->getTime()));
	}));
};

void SheetMusicComponent::init(void) {
	mLineGap = mHeight / 2 / 6;
	mBarGap = mWidth / 4;

	mTrebleClefTexture.load();
	mTrebleClefTexture.scaleToHeight(mHeight / 2);

	mBassClefTexture.load();
	mBassClefTexture.scaleToHeight(mHeight / 2 - 40);

	mQuaterNoteTexture.load();
}

void SheetMusicComponent::onClick(int button, int x, int y) {
}

void SheetMusicComponent::drawLines(void) {
	setColor(0, 0, 0);

	mTrebleClefTexture.render(0, 0);
	mBassClefTexture.render(0, mHeight / 2);

	for (int i = 1; i < 6; i++) {
		drawLine(0, i * mLineGap, mWidth, i * mLineGap);
	}
	for (int i = 7; i < 12; i++) {
		drawLine(0, i * mLineGap, mWidth, i * mLineGap);
	}

	for (int i = 1; i < 4; i++) {
		drawLine(mBarGap * i, 0, mBarGap * i, mHeight);
	}
}

void SheetMusicComponent::draw(void) {
	setColor(255, 255, 255);
	fillRectangle(mX, mY, mWidth, mHeight);

	drawLines();

	std::vector<Music::Note>& keys = mNotes;

	double barLength = 60.0 / (double)mRcdr->getBpm() * 4.0;
	double fullLength = barLength * 4;
	double startTime = std::floor(mRcdr->getTime() / fullLength) * fullLength;

	double now = mRcdr->getTime() - startTime;
//    double now = roundTo(mRcdr->getTime() - startTime, barLength / 4.0);

	double lineX = rmap(now, 0, fullLength, 0, mWidth);
	drawLine(lineX, 0, lineX, mHeight);

	if (keys.size() == 0) return;
//    return;

	for (int i = 0; i < keys.size(); i++) {
		Music::Note key = keys[i];
		if (key.time < startTime) continue;
		double time = roundTo(key.time - startTime, barLength / 4.0 / 4.0);
//        time = key.time - startTime;
		if (time > fullLength) {
			break;
		}

		setColor(0, 0, 0);
		int keyId = Music::getNoteId(key.note);
		mQuaterNoteTexture.render(rmap(time, 0, fullLength, 0, mWidth), (36 - keyId) * mLineGap / 2 - 5, 0.02);
	}
}
