#ifndef __MKBD_MIDI_TRACK_HPP__
#define __MKBD_MIDI_TRACK_HPP__

#include <mkbd/utils.hpp>
#include <mkbd/midi/keyboard.hpp>
#include <mkbd/midi/recorder.hpp>

#include <vector>

class MidiTrack {
	std::vector<MidiEvent> mEvents;
	double mStartTime = -1;
	double mCurrentTime = 0;
	MidiRecorder* mRcdr = 0;

public:
	MidiTrack(const std::vector<MidiEvent>& events)
	: mEvents(events) {};

	void attachRecorder(MidiRecorder* rcdr) { mRcdr = rcdr; };
	void emit(void);
	bool isNextEventReady(void);
	void reset(void) { mStartTime = 0; };
};

#endif