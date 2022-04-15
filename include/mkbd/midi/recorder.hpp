#ifndef __MKBD_RECORDER_HPP__
#define __MKBD_RECORDER_HPP__

#include <mkbd/midi/keyboard.hpp>
#include <mkbd/music.hpp>
#include <mkbd/timer.hpp>
#include <mkbd/utils.hpp>
#include <mkbd/eventemitter.hpp>

#include <vector>
#include <functional>

class MidiRecorder : public EventEmitter {
	struct TimedCallback {
		std::function<void(MidiRecorder* rcdr)> callback;
		double time;
		double full;
		FlipFlop flop;
		TimedCallback(std::function<void(MidiRecorder* rcdr)> callback, double time, double full)
		: callback(callback), time(time), full(full) {};
	};

	MidiDevice* mDevice;
	Timer mTimer;

	std::vector<MidiEvent> mEvents;

	std::vector<TimedCallback> mTimedCallbacks;

	int mBpm = 100;
	bool mStopping = false;
	bool mStarting = false;

public:
	MidiRecorder(void)
	: MidiRecorder(0) {};
	MidiRecorder(MidiDevice* device)
	: mDevice(device) {};
	MidiRecorder(MidiDevice* device, int bpm)
	: mDevice(device), mBpm(bpm) {};

	std::vector<MidiEvent> record(double time);
	void stop(void) { mStopping = true; };
	void clear(void) { mEvents.clear(); };
	void restart(void) { mStarting = true; clear(); };

	void sendEvent(MidiEvent e) { handleEvent(e); };

	void addTimedCallback(std::function<void(MidiRecorder* rcdr)> callback, double time, double full) {
		mTimedCallbacks.push_back(TimedCallback(callback, time, full));
	};

	MidiDevice* getDevice(void) { return mDevice; };
	std::vector<MidiEvent>& getEvents(void) { return mEvents; };

	Timer& getTimer(void) { return mTimer; };
	double getTime(void) { return mTimer.now(); };

	int getBpm(void) { return mBpm; };

private:
	bool isPadNote(int note);

	void handleEvent(MidiEvent e);
	void handleNoteOffEvent(MidiEvent e);
	void handleNoteOnEvent(MidiEvent e);
	void handleControlChangeEvents(MidiEvent e);
};

#endif