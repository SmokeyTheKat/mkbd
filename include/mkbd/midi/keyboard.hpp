#ifndef __MKBD_KEYBOARD_H__
#define __MKBD_KEYBOARD_H__

#include <RtMidi.h>
#include <mkbd/utils.hpp>

#include <vector>
#include <string>
#include <cstdint>

typedef std::vector<byte> RawMidiEvent;

struct MidiEvent {
	RawMidiEvent raw;
	double time;

	enum {
		NoteOff = 0x08,
		NoteOn = 0x09,
		AfterTouch = 0x0a,
		ControlChange = 0x0b,
		ProgramChange = 0x0c,
		PressureChannel = 0x0d,
		PitchWheel = 0x0e,
	};

	struct ControlType {
		enum {
			SoftPedal = 67,
			MiddlePedal = 66,
			SustainPedal = 64,
		};
	};

	MidiEvent(void)
	: MidiEvent({0}, 0) {};
	MidiEvent(const RawMidiEvent& data)
	: MidiEvent(data, 0) {};
	MidiEvent(const RawMidiEvent& data, double time)
	: raw(data), time(time) {
		if (raw.size() >= 1 && raw[0] < 0x10)
			raw[0] *= 0x10;
	};

	byte getType(void) {
		if (raw.size() == 0)
			return 0;
		return raw[0] / 0x10;
	};
	void setType(byte type) { raw[0] = type * 0x10 + getChannel(); };

	byte getChannel(void) {
		if (raw.size() == 0)
			return 0;
		return raw[0] % 0x10;
	};
	void setChannel(byte channel) { raw[0] = getType() * 0x10 + channel; };

	int length(void) { return raw.size(); };

	byte& operator[](int index) { 
		if (index >= raw.size()) raw.resize(index + 1);
		return raw[index];
	}
};

class MidiDevice {
public:
	struct Info {
		std::string name;
		int port;

		Info(void)
		: name(""), port(-1) {};
		Info(int port)
		: port(port), name("") {};
		Info(int port, std::string name)
		: port(port), name(name) {};
	};

private:
	RtMidiIn* mRtmIn;
	RtMidiOut* mRtmOut;
	Info mInfo;

	bool noteStates[255] = { 0 };

public:
	MidiDevice(int port);
	~MidiDevice(void);

	MidiEvent getEvent(void) const;
	void sendEvent(const MidiEvent& e) const;

	void setNoteState(byte note, bool state) { noteStates[note] = state; };
	bool getNoteState(byte note) const { return noteStates[note]; };

	static std::vector<Info> getDevices(void);

private:
	void adjustEvent(MidiEvent& e) const;
};

#endif