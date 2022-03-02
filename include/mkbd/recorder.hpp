#ifndef __MKBD_RECORDER_HPP__
#define __MKBD_RECORDER_HPP__

#include <mkbd/keyboard.hpp>
#include <mkbd/timer.hpp>

#include <vector>
#include <functional>

class KeyboardRecorder {
	Keyboard* mKeyboard;
	Timer mTimer;

	std::vector<Key> mNotes;
	std::vector<KeyboardMessage> mMessages;

	struct TimedCallback {
		std::function<void(KeyboardRecorder* rcdr)> callback;
		double time;
		double full;
		bool ran;
		inline TimedCallback(std::function<void(KeyboardRecorder* rcdr)> callback, double time, double full)
			: callback(callback), time(time), full(full), ran(false) {};
	};

	std::vector<TimedCallback> mTimedCallbacks;

	int mBpm = 100;
	bool mStopping = false;
	bool mStarting = false;

public:
	KeyboardRecorder(Keyboard* keyboard);
	KeyboardRecorder(Keyboard* keyboard, int bpm);

	std::vector<Key> record(double time);

	inline void addTimedCallback(std::function<void(KeyboardRecorder* rcdr)> callback, double time, double full) {
		mTimedCallbacks.push_back(TimedCallback(callback, time, full));
	};

	inline Keyboard* getKeyboard(void) { return mKeyboard; };
	inline std::vector<Key>& getNotes(void) { return mNotes; };
	inline std::vector<KeyboardMessage>& getMessages(void) { return mMessages; };
	inline Timer& getTimer(void) { return mTimer; };
	inline double getTime(void) { return mTimer.now(); };
	inline int getBpm(void) { return mBpm; };
	inline void stop(void) { mStopping = true; };
	inline void clear(void) { mNotes.clear(); mMessages.clear(); };
	inline void restart(void) { mStarting = true; clear(); };

	std::function<void(KeyboardRecorder* rcdr)> onBeat = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onMessage = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onPadDown = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onPadUp = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onKeyDown = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onKeyUp = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onSoftPedalDown = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onSoftPedalUp = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onMiddlePedalDown = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onMiddlePedalUp = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onSustainChange = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onSustainUp = 0;
	std::function<void(KeyboardRecorder* rcdr, KeyboardMessage msg)> onSustainDown = 0;

private:
	void handleMessage(KeyboardMessage msg);
};

#endif