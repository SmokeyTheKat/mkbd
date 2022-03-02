#include <mkbd/gui/graphics/sheetmusic.hpp>

#include <mkbd/math.hpp>

SheetMusicGraphic::SheetMusicGraphic(KeyboardRecorder* rcdr, int x, int y, int width, int height)
: Graphic(x, y, width, height), mRcdr(rcdr) {};

void SheetMusicGraphic::draw(void) {
	setColor(255, 255, 255);
	fillRectangle(mX, mY, mWidth, mHeight);
	setColor(0, 0, 0);
	int lineGap = mHeight / 2 / 6;
	for (int i = 1; i < 6; i++) {
		drawLine(0, i * lineGap, mWidth, i * lineGap);
	}
	for (int i = 7; i < 12; i++) {
		drawLine(0, i * lineGap, mWidth, i * lineGap);
	}

	int barGap = mWidth / 4;

	for (int i = 1; i < 4; i++) {
		drawLine(barGap * i, 0, barGap * i, mHeight);
	}

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
		time = key.time - startTime;
		if (time > fullLength) {
			break;
		}
//        if (key.id % 2 == 0) std::cout << "\x1b[9m";
//        else std::cout << "\x1b[0m";

		std::cout << rmap(time, 0, fullLength, 0, mWidth) << "\n";
		setColor(0, 0, 0);
		fillRectangle(rmap(time, 0, fullLength, 0, mWidth), (40 - key.id) * lineGap / 2 - 5, 10, 10);

//        std::cout << "\x1b[" <<  << ";"
//                  << (int)rmap(time, 0, fullLength, 0, mWidth) - (key.flat ? 0 : 0)
//                  << (key.flat ? "HO" : "H♭O");
	}
}
