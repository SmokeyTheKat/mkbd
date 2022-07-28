#ifndef __MKBD_MIDI_TRACK_HPP__
#define __MKBD_MIDI_TRACK_HPP__

#include <mkbd/utils.hpp>
#include <mkbd/midi/keyboard.hpp>
#include <mkbd/music.hpp>
#include <mkbd/midi/recorder.hpp>

#include <vector>

struct MidiTrack {
	int bpm = 120;
	Music::Meter meter;
	std::vector<MidiEvent> events;
	std::string name;
	std::string instrument;

	double getDuration(void) {
		double duration = 0;
		for (const MidiEvent& e : events) {
			duration += e.time / (60.0 * bpm);
		}
		return duration;
	};
};
//
//class MidiTrack {
//    std::vector<MidiEvent> mEvents;
//    double mCurrentTime = 0;
//    double mStartTime = 0;
//    MidiRecorder* mRcdr = 0;
//    double mBpm = 120;
//    double mBeatCount = 0;
//    int mPtr = 0;
//
//public:
//    MidiTrack(const std::vector<MidiEvent>& events)
//    : mEvents(events) {};
//    MidiTrack(const std::vector<MidiEvent>& events, double beatCount)
//    : mEvents(events), mBeatCount(beatCount) {};
//
//    void attachRecorder(MidiRecorder* rcdr) { mRcdr = rcdr; };
//    void resync(void);
//    void emit(void);
//    void reset(void) { mStartTime = mRcdr->getTime() + 2; mCurrentTime = mStartTime; };
//
//    std::vector<MidiEvent>& getEvents(void) { return mEvents; };
//    double getStartTime(void) const { return mStartTime; };
//    double getCurrentTime(void) const { return mRcdr->getTime(); };
//
//    double getLength(void);
//    double getTLength(double bpm) { return mBeatCount * (60.0 / mRcdr->getBpm()); };
//    double getBpm(void) { return mBpm; };
//
//    bool eventsPresent(void) { return mEvents.size() > 0; };
//    bool isNextEventReady(void);
//    bool isPlaying(void) { return mPtr < mEvents.size(); };
//    double getNextTime(void) { return mEvents[mPtr].time * (60.0 / mRcdr->getBpm()); };
//
//private:
//    void nextEvent(void);
//    void prevEvent(void);
//};

#endif