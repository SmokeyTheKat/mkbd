#include <mkbd/gui/graphics/sheetmusic.hpp>

SheetMusicGraphic::SheetMusicGraphic(std::vector<Key> notes, int x, int y, int width, int height)
: Graphic(x, y, width, height), mNotes(notes) {};

void SheetMusicGraphic::draw(void) {
	setColor(255, 255, 255);
	fillRectangle(mX, mY, mWidth, mHeight);
}
