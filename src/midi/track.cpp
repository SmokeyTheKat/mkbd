#include <mkbd/midi/track.hpp>
#include <unistd.h>
//
//bool MidiTrack::isNextEventReady(void) {
//    if (mRcdr == 0) return false;
//    return mCurrentTime >= 0 &&
//           eventsPresent() &&
//           mPtr >= 0 && 
//           mPtr < mEvents.size() &&
//           mRcdr->getTime() > mCurrentTime + getNextTime();
//}
//
//double MidiTrack::getLength(void) {
//    return mBeatCount * (60.0 / mRcdr->getBpm());
//}
//
//void MidiTrack::nextEvent(void) {
//    mCurrentTime += getNextTime();
//    mPtr++;
//}
//void MidiTrack::prevEvent(void) {
//    if (mPtr == 0) return;
//    mPtr--;
//    mCurrentTime -= getNextTime();
//}
//
//void MidiTrack::resync(void) {
//    if (!eventsPresent()) return;
//    while (mPtr > 0 && !isNextEventReady()) {
//        prevEvent();
//    }
//    while (isNextEventReady()) {
//        nextEvent();
//    }
//    nextEvent();
//}
//
//void MidiTrack::emit(void) {
//    if (mRcdr == 0) return;
//
//    mRcdr->sendEvent(mEvents[mPtr]);
//    nextEvent();
//}

