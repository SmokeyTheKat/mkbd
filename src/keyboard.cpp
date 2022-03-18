#include <mkbd/keyboard.hpp>

#include <mkbd/music.hpp>

#include <vector>
#include <cstring>
#include <cstdarg>
#include <unistd.h>

MidiDevice::MidiDevice(int port)
: mInfo(port) {
	mRtmIn = new RtMidiIn();
	mRtmIn->openPort(port);
	mRtmIn->ignoreTypes(false, false, false);

	mRtmOut = new RtMidiOut();
	mRtmOut->openPort(port);
}

MidiDevice::~MidiDevice(void) {
	delete mRtmIn;
	delete mRtmOut;
}

void MidiDevice::adjustEvent(MidiEvent& e) const {
	if (e.length() == 3) {
		if (e.getType() == MidiEvent::NoteOn && e[2] == 0) {
			e.setType(MidiEvent::NoteOff);
		}
	}
}

MidiEvent MidiDevice::getEvent(void) const {
	RawMidiEvent raw;
	double stamp = mRtmIn->getMessage(&raw);
	MidiEvent event(raw, stamp);
	adjustEvent(event);
	return event;
}

void MidiDevice::sendEvent(const MidiEvent& e) const {
	mRtmOut->sendMessage(&e.raw);
}

std::vector<MidiDevice::Info> MidiDevice::getDevices(void) {
	RtMidiIn* rtm = new RtMidiIn();
	int portCount = rtm->getPortCount();

	std::vector<MidiDevice::Info> devices;

	for (int port = 0; port < portCount; port++) {
		try {
			 devices.push_back(MidiDevice::Info(port, rtm->getPortName(port)));
		}
		catch (RtMidiError& error) {
			error.printMessage();
		}
	}

	delete rtm;

	return devices;
}
