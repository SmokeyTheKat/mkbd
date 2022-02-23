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
	this->port = port;

	rtmIn = new RtMidiIn();
	rtmIn->openPort(port);
	rtmIn->ignoreTypes(false, false, false);

	rtmOut = new RtMidiOut();
	rtmOut->openPort(port);
}

Keyboard::~Keyboard(void) {
//    delete rtmIn;
//    delete rtmOut;
}

KeyboardMessage Keyboard::getMessage(void) const {
	std::vector<byte> message;
	double stamp = rtmIn->getMessage(&message);
	if (message.size() == 3)
		return KeyboardMessage(message, stamp);
	else return KeyboardMessage();
}

void Keyboard::sendMessage(const KeyboardMessage& msg) const {
	rtmOut->sendMessage(&msg.data);
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
