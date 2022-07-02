#ifndef __MKBD_RECORDER_HPP__
#define __MKBD_RECORDER_HPP__

#include <mkbd/midi/keyboard.hpp>
#include <mkbd/music.hpp>
#include <mkbd/timer.hpp>
#include <mkbd/utils.hpp>
#include <mkbd/eventemitter.hpp>

#include <vector>
#include <array>
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

	using Notes = std::array<Music::Note, 255>;

	MidiDevice* mDevice;
	Timer mTimer;

	std::vector<MidiEvent> mEvents;
	std::vector<TimedCallback> mTimedCallbacks;
	Notes mNotes;
	Notes mSustainedNotes;

	int mBpm = 120;
	bool mStopping = false;
	bool mStarting = false;

	int mSustainLevel = 0;

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
	void restart(void) { mStarting = true; clear(); mTimer.reset(); };

	bool isSustaining(void) { return mSustainLevel > 0; };
	int getSustainLevel(void) { return mSustainLevel; };

	void resetNotes(void) { mNotes.fill(Music::Note::None()); mSustainedNotes.fill(Music::Note::None()); };
	Music::Note getNoteState(int note) { return mNotes[note]; };
	Music::Note getSustainedNoteState(int note) { return mSustainedNotes[note]; };
	Notes getNotes(void) { return mNotes; };
	Notes getSustainedNotes(void) { return mSustainedNotes; };

	void sendEvent(MidiEvent e) { handleEvent(e); };

	void addTimedCallback(std::function<void(MidiRecorder* rcdr)> callback, double time, double full) {
		mTimedCallbacks.push_back(TimedCallback(callback, time, full));
	};

	MidiDevice* getDevice(void) { return mDevice; };
	std::vector<MidiEvent>& getEvents(void) { return mEvents; };

	Timer& getTimer(void) { return mTimer; };
	double getTime(void) { return mTimer.now(); };

	double getBpm(void) { return mBpm; };
	void setBpm(int bpm) { mBpm = bpm; };

private:
	bool isPadNote(int note);

	void handleEvent(MidiEvent e);
	void handleNoteOffEvent(MidiEvent e);
	void handleNoteOnEvent(MidiEvent e);
	void handleControlChangeEvents(MidiEvent e);
};

#endif