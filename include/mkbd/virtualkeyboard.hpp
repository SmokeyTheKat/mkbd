#ifndef __MKBD_VIRTUALKEYBOARD_HPP__
#define __MKBD_VIRTUALKEYBOARD_HPP__

#include <mkbd/midi/recorder.hpp>
#include <mkbd/eventemitter.hpp>

#include <SDL2/SDL.h>

class VirtualKeyboard : public EventEmitter {
	MidiRecorder* mRcdr;

	int mSustainToggleKey = SDLK_CAPSLOCK;
	int mSustainHoldKey = ' ';

	bool mSustainToggle = true;
	bool mShifted = false;
	bool mSwitch = true;

	int mOffset = 0;
	int mMode = 0;

public:
	VirtualKeyboard(MidiRecorder* rcdr)
	: mRcdr(rcdr) {};

	int keyToNote(int key);

	void onKeyDown(int key);
	void onKeyUp(int key);
};

#endif