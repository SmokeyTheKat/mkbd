#include <mkbd/recorder.hpp>

#include <mkbd/music.hpp>
#include <mkbd/math.hpp>

#include <unistd.h>
#include <cstring>
#include <cmath>

#define runCallback(callback) { if (callback) callback(this); };
#define runMsgCallback(callback) { if (callback) callback(this, msg); };

KeyboardRecorder::KeyboardRecorder(Keyboard* keyboard)
	: mKeyboard(keyboard) { }

KeyboardRecorder::KeyboardRecorder(Keyboard* keyboard, int bpm)
	: mKeyboard(keyboard), mBpm(bpm) { }

void KeyboardRecorder::handleMessage(KeyboardMessage msg) {
	msg.stamp = mTimer.now();
	runMsgCallback(onMessage);
	switch (msg.getType()) {
		case KBD_MSG_KEY_UP: {
			mKeyboard->setKeyState(msg[1], 0);
			mMessages.push_back(msg);
			runMsgCallback(onKeyUp);
			if ((msg[1] >= 16 && msg[1] <= 23) || (msg[1] >= 32 && msg[1] <= 39)) {
				runMsgCallback(onPadUp);
			}
		} break;
		case KBD_MSG_KEY: {
			if (mStarting == true) {
				mTimer.start();
				clear();
				msg.stamp = mTimer.now();
			}
			mStarting = false;

			mKeyboard->setKeyState(msg[1], 1);
			mNotes.push_back(Key(msg[1], msg[2], mTimer.now()));
			mMessages.push_back(msg);

			runMsgCallback(onKeyDown);
			if ((msg[1] >= 16 && msg[1] <= 23) || (msg[1] >= 32 && msg[1] <= 39)) {
				runMsgCallback(onPadDown);
			}
		} break;
		case KBD_MSG_PEDAL: {
			mMessages.push_back(msg);
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
	mStarting = true;
	bool beat = false;
	while (!mStopping && (time == 0 || mStarting || mTimer.now() < time)) {
		for (auto& callback : mTimedCallbacks) {
			double wow = std::fmod(mTimer.now(), callback.full);
			if (isAbout(wow, callback.time, 0.01)) {
				if (!callback.ran) {
					runCallback(callback.callback);
				}
				callback.ran = true;
			} else {
				callback.ran = false;
			}
		}

		if (std::fmod(mTimer.now(), 60.0 / mBpm) < 0.01) {
			if (!beat) {
				runCallback(onBeat);
			}
			beat = true;
		} else {
			beat = false;
		}

		handleMessage(mKeyboard->getMessage());
		usleep(100);
	}
	return mNotes;
}
