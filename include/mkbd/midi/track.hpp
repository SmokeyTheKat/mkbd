#ifndef __MKBD_MIDI_TRACK_HPP__
#define __MKBD_MIDI_TRACK_HPP__

#include <mkbd/utils.hpp>
#include <mkbd/midi/keyboard.hpp>
#include <mkbd/midi/recorder.hpp>

#include <vector>

class MidiTrack {
	std::vector<MidiEvent> mEvents;
	double mCurrentTime = 0;
	MidiRecorder* mRcdr = 0;
	double mBpm = 120;

public:
	MidiTrack(const std::vector<MidiEvent>& events)
	: mEvents(events) {};

	void attachRecorder(MidiRecorder* rcdr) { mRcdr = rcdr; };
	void emit(void);
	bool isNextEventReady(void);
	void reset(void) { mCurrentTime = mRcdr->getTime(); };
	double getNextTime(void) { return mEvents.front().time * (60.0 / mRcdr->getBpm()); };
};

#endif