#ifndef __MKBD_GUI_SHEET_MUSIC_HPP__
#define __MKBD_GUI_SHEET_MUSIC_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/keyboard.hpp>
#include <mkbd/recorder.hpp>

class SheetMusicGraphic : public Graphic {
	KeyboardRecorder* mRcdr;

public:
	SheetMusicGraphic(KeyboardRecorder* rcdr, int x, int y, int width, int height);
	void draw(void);
	
private:
};

#endif