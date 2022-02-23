#include <mkbd/recorder.hpp>

#include <mkbd/music.hpp>

#include <unistd.h>
#include <cstring>

#define runCallback(callback) { if (callback) callback(this, msg); };

KeyboardRecorder::KeyboardRecorder(Keyboard* keyboard)
	: keyboard(keyboard) { }

KeyboardRecorder::KeyboardRecorder(Keyboard* keyboard, int bpm)
	: keyboard(keyboard), bpm(bpm) { }

void KeyboardRecorder::handleMessage(KeyboardMessage msg) {
	switch (msg.getType()) {
		case KBD_MSG_KEY_UP: {
			keyboard->setKeyState(msg[1], 0);
			runCallback(onKeyUp);
		} break;
		case KBD_MSG_KEY: {
			if (starting == true) timer.start();
			starting = false;

			if (msg[2] == 0) {
				keyboard->setKeyState(msg[1], 0);
				runCallback(onKeyUp);
			} else {
				keyboard->setKeyState(msg[1], 1);
				notes.push_back(Key(msg[1], msg[2], timer.now()));
				runCallback(onKeyDown);
			}
		} break;
		case KBD_MSG_PEDAL: {
			if (msg[1] == 67) {
				if (msg[2] == 0) {
					runCallback(onSoftPedalUp);
				} else {
					runCallback(onSoftPedalDown);
				}
			}
			else if (msg[1] == 66) {
				if (msg[2] == 0) {
					runCallback(onMiddlePedalUp);
				} else {
					runCallback(onMiddlePedalDown);
				}
			}
			else if (msg[1] == 64) {
				runCallback(onSustainChange);
				if (msg[2] == 0) {
					runCallback(onSustainUp);
				} else if (msg[2] == 127) {
					runCallback(onSustainDown);
				}
			}
		} break;
	}
}

std::vector<Key> KeyboardRecorder::record(double time) {
	starting = true;
	while (!stopping && (time == 0 || starting || timer.now() < time)) {
		handleMessage(keyboard->getMessage());
		usleep(100);
	}
	return this->notes;
}
