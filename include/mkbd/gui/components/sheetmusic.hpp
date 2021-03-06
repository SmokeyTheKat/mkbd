#ifndef __MKBD_GUI_SHEET_MUSIC_HPP__
#define __MKBD_GUI_SHEET_MUSIC_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/texture.hpp>
#include <mkbd/midi/keyboard.hpp>
#include <mkbd/midi/recorder.hpp>
#include <mkbd/music.hpp>

class SheetMusicComponent : public Component {
	MidiRecorder* mRcdr;
	Texture mTrebleClefTexture;
	Texture mBassClefTexture;
	Texture mQuaterNoteTexture;
	int mLineGap;
	int mBarGap;
	std::vector<Music::Note> mNotes;

public:
	SheetMusicComponent(Layout layout, MidiRecorder* rcdr);
	void draw(void);
	void init(void);
	void onClick(int button, int x, int y);
	
private:
	void drawLines(void);
};

#endif