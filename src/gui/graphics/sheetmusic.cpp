#include <mkbd/gui/graphics/sheetmusic.hpp>

#include <mkbd/gui/window.hpp>
#include <mkbd/math.hpp>

SheetMusicGraphic::SheetMusicGraphic(int x, int y, int width, int height, KeyboardRecorder* rcdr)
: Graphic(x, y, width, height), mRcdr(rcdr), mLineGap(height / 2 / 6), mBarGap(width / 4),
  mTrebleClefTexture("./trebleclef.png"), mQuaterNoteTexture("./quarternote.png"),
  mBassClefTexture("./bassclef.png") {};

void SheetMusicGraphic::init(void) {
	mTrebleClefTexture.setRenderer(getRenderer());
	mTrebleClefTexture.load();
	mTrebleClefTexture.scaleToHeight(mHeight / 2);

	mBassClefTexture.setRenderer(getRenderer());
	mBassClefTexture.load();
	mBassClefTexture.scaleToHeight(mHeight / 2 - 40);

	mQuaterNoteTexture.setRenderer(getRenderer());
	mQuaterNoteTexture.load();
}

void SheetMusicGraphic::onClick(int button, int x, int y) {
	std::cout << "mouse " << button << " down [" << x << ", " << y << "]\n";
}

void SheetMusicGraphic::drawLines(void) {
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

void SheetMusicGraphic::draw(void) {
	setColor(255, 255, 255);
	fillRectangle(mX, mY, mWidth, mHeight);

	drawLines();

	std::vector<Key>& keys = mRcdr->getNotes();

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
		Key key = keys[i];
		if (key.time < startTime) continue;
		double time = roundTo(key.time - startTime, barLength / 4.0 / 4.0);
//        time = key.time - startTime;
		if (time > fullLength) {
			break;
		}
//        if (key.id % 2 == 0) std::cout << "\x1b[9m";
//        else std::cout << "\x1b[0m";

		std::cout << rmap(time, 0, fullLength, 0, mWidth) << "\n";
		setColor(0, 0, 0);
		mQuaterNoteTexture.render(rmap(time, 0, fullLength, 0, mWidth), (36 - key.id) * mLineGap / 2 - 5, 0.02);
//        fillRectangle(rmap(time, 0, fullLength, 0, mWidth), (40 - key.id) * mLineGap / 2 - 5, 10, 10);

//        std::cout << "\x1b[" <<  << ";"
//                  << (int)rmap(time, 0, fullLength, 0, mWidth) - (key.flat ? 0 : 0)
//                  << (key.flat ? "HO" : "H♭O");
	}
}
