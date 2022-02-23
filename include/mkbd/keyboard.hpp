#ifndef __MKBD_KEYBOARD_H__
#define __MKBD_KEYBOARD_H__

#include <RtMidi.h>
#include <mkbd/utils.hpp>

#include <vector>
#include <string>
#include <cstdint>

enum {
	KBD_MSG_KEY=144,
	KBD_MSG_KEY_UP=128,
	KBD_MSG_PEDAL=176,
	KBD_MSG_PRGM_CHANGE=192,
};

struct KeyboardInfo {
	std::string name;
	int port;

	KeyboardInfo(void);
	KeyboardInfo(int port, std::string name);
};

struct KeyboardMessage {
	std::vector<byte> data;
	double stamp;

	KeyboardMessage(void);
	KeyboardMessage(const std::vector<byte>& msg, double stamp);
	KeyboardMessage(int byte_count, ...);

	inline byte getType(void) const { return data.size() > 0 ? data[0] : 0; };
	inline void setType(byte type) { data[0] = type; };
	inline byte& operator[](int index) { 
		if (index > data.size()) data.resize(index + 4);
		return data[index];
	}
};

struct Key {
	byte key;
	int id;
	bool flat;
	byte velocity;
	double time;

	Key(byte key, byte velocity, double time);
};

class Keyboard {
	RtMidiIn* rtmIn;
	RtMidiOut* rtmOut;
	int port;
	byte keys[256] = {0};

public:
	Keyboard(int port);
	~Keyboard(void);

	KeyboardMessage getMessage(void) const;
	void sendMessage(const KeyboardMessage& msg) const;

	inline void setKeyState(byte key, int state) { keys[key] = state; };
	inline int getKeyState(byte key) const { return keys[key]; };
	inline int getKeyCount(void) const { return 88; };

	static std::vector<KeyboardInfo> getKeyboards(void);
};

#endif