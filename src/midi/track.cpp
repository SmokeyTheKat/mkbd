#include <mkbd/midi/track.hpp>
#include <unistd.h>

bool MidiTrack::isNextEventReady(void) {
	if (mRcdr == 0) return false;
	return mCurrentTime >= 0 &&
		   mEvents.size() > 0 &&
		   mRcdr->getTime() > mCurrentTime + getNextTime();
}

void MidiTrack::emit(void) {
	if (mRcdr == 0) return;

	mCurrentTime += getNextTime();
	mRcdr->sendEvent(mEvents[mPtr++]);
}

