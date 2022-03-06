#ifndef __MKBD_GUI_SHEET_MUSIC_HPP__
#define __MKBD_GUI_SHEET_MUSIC_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/texture.hpp>
#include <mkbd/keyboard.hpp>
#include <mkbd/recorder.hpp>

class SheetMusicGraphic : public Graphic {
	KeyboardRecorder* mRcdr;
	Texture mTrebleClefTexture;
	Texture mBassClefTexture;
	Texture mQuaterNoteTexture;
	int mLineGap;
	int mBarGap;

public:
	SheetMusicGraphic(Layout layout, KeyboardRecorder* rcdr);
	void draw(void);
	void init(void);
	void onClick(int button, int x, int y);
	
private:
	void drawLines(void);
};

#endif