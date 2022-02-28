#ifndef __MKBD_RECORDER_HPP__
#define __MKBD_RECORDER_HPP__

#include <mkbd/keyboard.hpp>
#include <mkbd/timer.hpp>

#include <vector>
#include <functional>

class KeyboardRecorder {
	Keyboard* keyboard;
	Timer timer;

	std::vector<Key> notes;
	std::vector<KeyboardMessage> messages;

	struct TimedCallback {
		std::function<void(KeyboardRecorder* rcdr)> callback;
		double time;
		double full;
		bool ran;
		inline TimedCallback(std::function<void(KeyboardRecorder* rcdr)> callback, double time, double full)
			: callback(callback), time(time), full(full), ran(false) {};
	};

	std::vector<TimedCallback> timedCallbacks;

	int bpm = 100;
	bool stopping = false;
	bool starting = false;

public:
	KeyboardRecorder(Keyboard* keyboard);
	KeyboardRecorder(Keyboard* keyboard, int bpm);

	std::vector<Key> record(double time);

	inline void addTimedCallback(std::function<void(KeyboardRecorder* rcdr)> callback, double time, double full) {
		timedCallbacks.push_back(TimedCallback(callback, time, full));
	};

	inline Keyboard* getKeyboard(void) { return keyboard; };
	inline std::vector<Key>& getNotes(void) { return notes; };
	inline std::vector<KeyboardMessage>& getMessages(void) { return messages; };
	inline Timer& getTimer(void) { return timer; };
	inline double getTime(void) { return timer.now(); };
	inline int getBpm(void) { return bpm; };
	inline void stop(void) { this->stopping = true; };
	inline void clear(void) { notes.clear(); messages.clear(); };
	inline void restart(void) { this->starting = true; clear(); };

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