#include <mkbd/keyboard.hpp>

#include <mkbd/music.hpp>

#include <vector>
#include <cstring>
#include <cstdarg>
#include <unistd.h>

KeyboardInfo::KeyboardInfo(int port, std::string name) {
	this->port = port;
	this->name = name;
}

KeyboardMessage::KeyboardMessage(void) {
	data.reserve(128);
	this->stamp = 0;
}

KeyboardMessage::KeyboardMessage(const std::vector<byte>& msg, double stamp) {
	data = msg;
	this->stamp = stamp;
}

KeyboardMessage::KeyboardMessage(int byte_count, ...) {
	this->stamp = 0;
	data.reserve(byte_count);

	std::va_list args;
	va_start(args, byte_count);

	for (int i = 0; i < byte_count; i++)
		data.push_back(va_arg(args, int));

	va_end(args);
}

KeyboardInfo::KeyboardInfo(void) {
	this->port = -1;
	this->name = "";
}

Key::Key(byte key, byte velocity, double time) {
	this->key = key;
	this->velocity = velocity;
	this->time = time;
	this->id = ((getKeyNameFromKey(key)[0] - 'A' + 5) % 7) + (7 * (getKeyOctiveFromKey(key)));
	this->flat = std::strlen(getKeyNameFromKey(key)) != 2;
}

Keyboard::Keyboard(int port) {
	mPort = port;

	mRtmIn = new RtMidiIn();
	mRtmIn->openPort(port);
	mRtmIn->ignoreTypes(false, false, false);

	mRtmOut = new RtMidiOut();
	mRtmOut->openPort(port);
}

Keyboard::~Keyboard(void) {
	delete mRtmIn;
	delete mRtmOut;
}

void Keyboard::adjustMessage(std::vector<byte>& msg) const {
	if (msg.size() == 3) {
		if (msg[0] == KBD_MSG_KEY && msg[2] == 0) {
			msg[0] = KBD_MSG_KEY_UP;
		}
	}
}

KeyboardMessage Keyboard::getMessage(void) const {
	std::vector<byte> message;
	double stamp = mRtmIn->getMessage(&message);
	adjustMessage(message);
	return KeyboardMessage(message, stamp);
}

void Keyboard::sendMessage(const KeyboardMessage& msg) const {
	mRtmOut->sendMessage(&msg.data);
}

std::vector<KeyboardInfo> Keyboard::getKeyboards(void) {
	RtMidiIn* _rtm = new RtMidiIn();
	int portCount = _rtm->getPortCount();

	std::vector<KeyboardInfo> keyboards;

	for (int port = 0; port < portCount; port++) {
		try {
			 keyboards.push_back(KeyboardInfo(port, _rtm->getPortName(port)));
		}
		catch (RtMidiError& error) {
			error.printMessage();
		}
	}

	delete _rtm;

	return keyboards;
}
