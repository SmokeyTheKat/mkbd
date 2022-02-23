#ifndef __MKBD_RECORDER_HPP__
#define __MKBD_RECORDER_HPP__

#include <mkbd/keyboard.hpp>
#include <mkbd/timer.hpp>

#include <vector>

class KeyboardRecorder {
	Keyboard* keyboard;
	std::vector<Key> notes;
	Timer timer;
	int bpm = 100;
	bool stopping = false;
	bool starting = false;

public:
	KeyboardRecorder(Keyboard* keyboard);
	KeyboardRecorder(Keyboard* keyboard, int bpm);

	std::vector<Key> record(double time);

	inline Keyboard* getKeyboard(void) { return keyboard; };
	inline std::vector<Key>& getNotes(void) { return notes; };
	inline Timer& getTimer(void) { return timer; };
	inline int getBpm(void) { return bpm; };
	inline void stop(void) { this->stopping = true; };
	inline void restart(void) { this->starting = true; notes.clear(); };

	void(*onKeyDown)(KeyboardRecorder* rcdr, KeyboardMessage msg) = 0;
	void(*onKeyUp)(KeyboardRecorder* rcdr, KeyboardMessage msg) = 0;
	void(*onSoftPedalDown)(KeyboardRecorder* rcdr, KeyboardMessage msg) = 0;
	void(*onSoftPedalUp)(KeyboardRecorder* rcdr, KeyboardMessage msg) = 0;
	void(*onMiddlePedalDown)(KeyboardRecorder* rcdr, KeyboardMessage msg) = 0;
	void(*onMiddlePedalUp)(KeyboardRecorder* rcdr, KeyboardMessage msg) = 0;
	void(*onSustainChange)(KeyboardRecorder* rcdr, KeyboardMessage msg) = 0;
	void(*onSustainUp)(KeyboardRecorder* rcdr, KeyboardMessage msg) = 0;
	void(*onSustainDown)(KeyboardRecorder* rcdr, KeyboardMessage msg) = 0;

private:
	void handleMessage(KeyboardMessage msg);
};

#endif