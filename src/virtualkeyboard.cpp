#include <mkbd/virtualkeyboard.hpp>

#include <cstring>

int VirtualKeyboard::keyToNote(int key) {
	static int convert[255] = {0};
	if (mSwitch) {
		mSwitch = false;
		for (int i = 0; i < 255; i++)
			convert[i] = 0;

		if (mMode == 1) {
			int i = 36;
			convert['1'] = i++;
			i++;
			convert['2'] = i++;
			i++;
			convert['3'] = i++;
			convert['4'] = i++;
			i++;
			convert['5'] = i++;
			i++;
			convert['6'] = i++;
			i++;
			convert['7'] = i++;
			convert['8'] = i++;
			i++;
			convert['9'] = i++;
			i++;
			convert['0'] = i++;
			convert['q'] = i++;
			i++;
			convert['w'] = i++;
			i++;
			convert['e'] = i++;
			i++;
			convert['r'] = i++;
			convert['t'] = i++;
			i++;
			convert['y'] = i++;
			i++;
			convert['u'] = i++;
			convert['i'] = i++;
			i++;
			convert['o'] = i++;
			i++;
			convert['p'] = i++;
			i++;
			convert['a'] = i++;
			convert['s'] = i++;
			i++;
			convert['d'] = i++;
			i++;
			convert['f'] = i++;
			convert['g'] = i++;
			i++;
			convert['h'] = i++;
			i++;
			convert['j'] = i++;
			i++;
			convert['k'] = i++;
			convert['l'] = i++;
			i++;
			convert['z'] = i++;
			i++;
			convert['x'] = i++;
			convert['c'] = i++;
			i++;
			convert['v'] = i++;
			i++;
			convert['b'] = i++;
			i++;
			convert['n'] = i++;
			convert['m'] = i++;
		} else if (mMode == 0) {
			int i = 48;
			convert['q'] = i++;
			convert['2'] = i++;
			convert['w'] = i++;
			convert['3'] = i++;
			convert['e'] = i++;
			convert['r'] = i++;
			convert['5'] = i++;
			convert['t'] = i++;
			convert['6'] = i++;
			convert['y'] = i++;
			convert['7'] = i++;
			convert['u'] = i++;
			convert['i'] = i++;
			convert['9'] = i++;
			convert['o'] = i++;
			convert['0'] = i++;
			convert['p'] = i++;
			convert['z'] = i++;
			convert['s'] = i++;
			convert['x'] = i++;
			convert['d'] = i++;
			convert['c'] = i++;
			convert['f'] = i++;
			convert['v'] = i++;
			convert['b'] = i++;
			convert['h'] = i++;
			convert['n'] = i++;
			convert['j'] = i++;
			convert['m'] = i++;
			convert[','] = i++;
			convert['l'] = i++;
			convert['.'] = i++;
			convert[';'] = i++;
			convert['/'] = i++;
			convert['\''] = i++;
		}
	}

	int note = convert[key] + mOffset;

	if (mShifted)
		note++;

	return note;
}

void VirtualKeyboard::onKeyDown(int key) {
	if (key == SDLK_LSHIFT) {
		mShifted = true;
	} else if (key == SDLK_TAB) {
		mMode = !mMode;
		mSwitch = true;
	} else if (key == mSustainToggleKey) {
		if (mSustainToggle)
			mRcdr->sendEvent(MidiEvent({MidiEvent::ControlChange, MidiEvent::ControlType::SustainPedal, 127}));
		else
			mRcdr->sendEvent(MidiEvent({MidiEvent::ControlChange, MidiEvent::ControlType::SustainPedal, 0}));
		mSustainToggle = !mSustainToggle;
	} else if (key == '=') {
		mOffset++;
	} else if (key == '-') {
		mOffset--;
	} else if (key == mSustainHoldKey) {
		mRcdr->sendEvent(MidiEvent({MidiEvent::ControlChange, MidiEvent::ControlType::SustainPedal, 127}));
	} else if (key <= 128) {
		int note = keyToNote(key);
		if (note == 0) return;
	
		mRcdr->sendEvent(MidiEvent({MidiEvent::NoteOn, note, 50}));
	}

}

void VirtualKeyboard::onKeyUp(int key) {
	if (key == SDLK_LSHIFT) {
		mShifted = false;
	} else if (key == mSustainToggleKey) {
		mRcdr->sendEvent(MidiEvent({MidiEvent::ControlChange, MidiEvent::ControlType::SustainPedal, 0}));
	} else if (key <= 128) {
		int note = keyToNote(key);
		if (note > 0)
			mRcdr->sendEvent(MidiEvent({MidiEvent::NoteOff, note, 0}));
	}
}
