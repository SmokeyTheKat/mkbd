#ifndef __MKBD_GUI_SHEET_MUSIC_HPP__
#define __MKBD_GUI_SHEET_MUSIC_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/keyboard.hpp>

class SheetMusicGraphic : public Graphic {
	std::vector<Key> mNotes;

public:
	SheetMusicGraphic(std::vector<Key> notes, int x, int y, int width, int height);
	void draw(void);
	
private:
};

#endif