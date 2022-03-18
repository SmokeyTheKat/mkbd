#ifndef __MKBD_RECORDER_HPP__
#define __MKBD_RECORDER_HPP__

#include <mkbd/keyboard.hpp>
#include <mkbd/music.hpp>
#include <mkbd/timer.hpp>

#include <vector>
#include <functional>

class MidiRecorder {
	MidiDevice* mDevice;
	Timer mTimer;

	std::vector<Music::Note> mNotes;
	std::vector<MidiEvent> mEvents;

	struct TimedCallback {
		std::function<void(MidiRecorder* rcdr)> callback;
		double time;
		double full;
		bool ran;
		TimedCallback(std::function<void(MidiRecorder* rcdr)> callback, double time, double full)
		: callback(callback), time(time), full(full), ran(false) {};
	};

	std::vector<TimedCallback> mTimedCallbacks;

	int mBpm = 100;
	bool mStopping = false;
	bool mStarting = false;

public:
	MidiRecorder(MidiDevice* device)
	: mDevice(device) {};
	MidiRecorder(MidiDevice* device, int bpm)
	: mDevice(device), mBpm(bpm) {};

	std::vector<Music::Note> record(double time);
	void stop(void) { mStopping = true; };
	void clear(void) { mNotes.clear(); mEvents.clear(); };
	void restart(void) { mStarting = true; clear(); };

	void sendEvent(MidiEvent e) { handleEvent(e); };

	void addTimedCallback(std::function<void(MidiRecorder* rcdr)> callback, double time, double full) {
		mTimedCallbacks.push_back(TimedCallback(callback, time, full));
	};

	MidiDevice* getDevice(void) { return mDevice; };
	std::vector<Music::Note>& getNotes(void) { return mNotes; };
	std::vector<MidiEvent>& getEvents(void) { return mEvents; };

	Timer& getTimer(void) { return mTimer; };
	double getTime(void) { return mTimer.now(); };

	int getBpm(void) { return mBpm; };

	std::function<void(MidiRecorder* rcdr)> onBeat = 0;
	std::function<void(MidiRecorder* rcdr)> onUpdate = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onMessage = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onPadDown = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onPadUp = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onKeyDown = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onKeyUp = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onSoftPedalDown = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onSoftPedalUp = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onMiddlePedalDown = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onMiddlePedalUp = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onSustainChange = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onSustainUp = 0;
	std::function<void(MidiRecorder* rcdr, MidiEvent msg)> onSustainDown = 0;

private:
	void handleEvent(MidiEvent e);
	void handleNoteOffEvent(MidiEvent e);
	void handleNoteOnEvent(MidiEvent e);
	void handleControlChangeEvents(MidiEvent e);
};

#endif