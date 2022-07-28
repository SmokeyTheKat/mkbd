#ifndef __MKBD_KEYBOARD_HPP__
#define __MKBD_KEYBOARD_HPP__

#include <RtMidi.h>
#include <mkbd/utils.hpp>
#include <mkbd/midi/event.hpp>

#include <vector>
#include <string>
#include <cstdint>

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

public:
	MidiDevice(int port);
	~MidiDevice(void);

	MidiEvent getEvent(void) const;
	void sendEvent(const MidiEvent& e) const;

	static std::vector<Info> getDevices(void);

private:
	void adjustEvent(MidiEvent& e) const;
};

#endif