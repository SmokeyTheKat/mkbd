#include <mkbd/recorder.hpp>

#include <mkbd/music.hpp>
#include <mkbd/math.hpp>

#include <unistd.h>
#include <cstring>
#include <cmath>

#define runCallback(callback) { if (callback) callback(this); };
#define runMsgCallback(callback) { if (callback) callback(this, msg); };

KeyboardRecorder::KeyboardRecorder(Keyboard* keyboard)
	: keyboard(keyboard) { }

KeyboardRecorder::KeyboardRecorder(Keyboard* keyboard, int bpm)
	: keyboard(keyboard), bpm(bpm) { }

void KeyboardRecorder::handleMessage(KeyboardMessage msg) {
	msg.stamp = timer.now();
	runMsgCallback(onMessage);
	switch (msg.getType()) {
		case KBD_MSG_KEY_UP: {
			keyboard->setKeyState(msg[1], 0);
			runMsgCallback(onKeyUp);
			messages.push_back(msg);
		} break;
		case KBD_MSG_KEY: {
			if (starting == true) {
				timer.start();
				clear();
				msg.stamp = timer.now();
			}
			starting = false;

			if (msg[2] == 0) {
				msg.setType(KBD_MSG_KEY_UP);
				keyboard->setKeyState(msg[1], 0);
				runMsgCallback(onKeyUp);
				messages.push_back(msg);
			} else {
				keyboard->setKeyState(msg[1], 1);
				notes.push_back(Key(msg[1], msg[2], timer.now()));
				runMsgCallback(onKeyDown);
				messages.push_back(msg);
			}
		} break;
		case KBD_MSG_PEDAL: {
			messages.push_back(msg);
			if (msg[1] == 67) {
				if (msg[2] == 0) {
					runMsgCallback(onSoftPedalUp);
				} else {
					runMsgCallback(onSoftPedalDown);
				}
			}
			else if (msg[1] == 66) {
				if (msg[2] == 0) {
					runMsgCallback(onMiddlePedalUp);
				} else {
					runMsgCallback(onMiddlePedalDown);
				}
			}
			else if (msg[1] == 64) {
				runMsgCallback(onSustainChange);
				if (msg[2] == 0) {
					runMsgCallback(onSustainUp);
				} else if (msg[2] == 127) {
					runMsgCallback(onSustainDown);
				}
			}
		} break;
	}
}

std::vector<Key> KeyboardRecorder::record(double time) {
	starting = true;
	bool beat = false;
	while (!stopping && (time == 0 || starting || timer.now() < time)) {
		for (auto& callback : timedCallbacks) {
			double wow = std::fmod(timer.now(), callback.full);
			if (isAbout(wow, callback.time, 0.01)) {
				if (!callback.ran) {
					runCallback(callback.callback);
				}
				callback.ran = true;
			} else {
				callback.ran = false;
			}
		}

		if (std::fmod(timer.now(), 60.0 / bpm) < 0.01) {
			if (!beat) {
				runCallback(onBeat);
			}
			beat = true;
		} else {
			beat = false;
		}

		handleMessage(keyboard->getMessage());
		usleep(100);
	}
	return this->notes;
}
