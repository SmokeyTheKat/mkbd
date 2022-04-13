#include <mkbd/midi/recorder.hpp>

#include <mkbd/music.hpp>
#include <mkbd/midi/recorder.hpp>
#include <mkbd/math.hpp>

#include <unistd.h>
#include <cstring>
#include <cmath>

#define runCallback(callback) { if (callback) callback(this); };
#define runMsgCallback(callback) { if (callback) callback(this, e); };

bool MidiRecorder::isPadNote(int note) {
	return (note >= 16 && note <= 23) || (note >= 32 && note <= 39);
}

void MidiRecorder::handleNoteOffEvent(MidiEvent e) {
	mDevice->setNoteState(e[1], 0);

	emit("NoteOff", e[1]);

	if (isPadNote(e[1])) {
		emit("PadOff", e[1], e[2]);
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
	
	emit("NoteOn", e[1], e[2]);

	if (isPadNote(e[1])) {
		emit("PadOn", e[1], e[2]);
	}
}

void MidiRecorder::handleControlChangeEvents(MidiEvent e) {
	mEvents.push_back(e);
	switch (e[1]) {
		case MidiEvent::ControlType::SoftPedal: {
			if (e[2] == 0) {
				emit("SoftPedalUp");
			} else {
				emit("SoftPedalDown");
			}
		} break;
		case MidiEvent::ControlType::MiddlePedal: {
			if (e[2] == 0) {
				emit("MiddlePedalUp");
			} else {
				emit("MiddlePedalDown");
			}
		} break;
		case MidiEvent::ControlType::SustainPedal: {
			emit("SustainChange", e[2]);
			if (e[2] == 0) {
				emit("SustainPedalUp");
			} else if (e[2] == 127) {
				emit("SustainPedalDown");
			}
		} break;
	}
}

void MidiRecorder::handleEvent(MidiEvent e) {
	if (e.length() == 0) return;
	e.time = mTimer.now();
	emit("Event", e);
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

std::vector<MidiEvent> MidiRecorder::record(double time) {
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
				emit("Beat");
			}
			beat = true;
		} else {
			beat = false;
		}
		emit("Update");
		handleEvent(mDevice->getEvent());
		usleep(100);
	}

	return mEvents;
}

