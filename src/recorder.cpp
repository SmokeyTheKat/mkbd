#include <mkbd/recorder.hpp>

#include <mkbd/music.hpp>
#include <mkbd/recorder.hpp>
#include <mkbd/math.hpp>

#include <unistd.h>
#include <cstring>
#include <cmath>

#define runCallback(callback) { if (callback) callback(this); };
#define runMsgCallback(callback) { if (callback) callback(this, e); };

void MidiRecorder::handleNoteOffEvent(MidiEvent e) {
	mDevice->setNoteState(e[1], 0);

	runMsgCallback(onKeyUp);

	if ((e[1] >= 16 && e[1] <= 23) || (e[1] >= 32 && e[1] <= 39)) {
		runMsgCallback(onPadUp);
	}
}

void MidiRecorder::handleNoteOnEvent(MidiEvent e) {
	if (mStarting == true) {
		mTimer.start();
		clear();
		e.time = mTimer.now();
	}
	mStarting = false;
	
	mDevice->setNoteState(e[1], 1);

	mNotes.push_back(Music::Note(e[1], e[2], mTimer.now()));
	
	runMsgCallback(onKeyDown);

	if ((e[1] >= 16 && e[1] <= 23) || (e[1] >= 32 && e[1] <= 39)) {
		runMsgCallback(onPadDown);
	}
}

void MidiRecorder::handleControlChangeEvents(MidiEvent e) {
	mEvents.push_back(e);
	if (e[1] == 67) {
		if (e[2] == 0) {
			runMsgCallback(onSoftPedalUp);
		} else {
			runMsgCallback(onSoftPedalDown);
		}
	}
	else if (e[1] == 66) {
		if (e[2] == 0) {
			runMsgCallback(onMiddlePedalUp);
		} else {
			runMsgCallback(onMiddlePedalDown);
		}
	}
	else if (e[1] == 64) {
		runMsgCallback(onSustainChange);
		if (e[2] == 0) {
			runMsgCallback(onSustainUp);
		} else if (e[2] == 127) {
			runMsgCallback(onSustainDown);
		}
	}
}

void MidiRecorder::handleEvent(MidiEvent e) {
	if (e.length() == 0) return;
	e.time = mTimer.now();
	runMsgCallback(onMessage);
	mEvents.push_back(e);
	switch (e.getType()) {
		case MidiEvent::NoteOff: {
			handleNoteOffEvent(e);
		} break;
		case MidiEvent::NoteOn: {
			handleNoteOnEvent(e);
		} break;
		case MidiEvent::ControlChange: {
			handleControlChangeEvents(e);
		} break;
	}
}

std::vector<Music::Note> MidiRecorder::record(double time) {
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
		runCallback(onUpdate);
		handleEvent(mDevice->getEvent());
		usleep(100);
	}

	return mNotes;
}
