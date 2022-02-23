#include <mkbd/player.hpp>

#include <mkbd/timer.hpp>

#include <unistd.h>

KeyboardPlayer::KeyboardPlayer(Keyboard* keybaord, std::vector<Key> notes, int bpm) {
	this->keyboard = keyboard;
	this->notes = notes;
	this->bpm = bpm;
}

void KeyboardPlayer::play(Keyboard* keyboard) {
	Timer timer;
	timer.start();
	Key prev = notes.front();
	while (notes.size() > 0) {
		Key next = notes.front();
		while (timer.now() < next.time) usleep(100);
//        keyboard->sendMessage(KeyboardMessage(3, KBD_MSG_KEY_UP, prev.key, 0));
		keyboard->sendMessage(KeyboardMessage(3, KBD_MSG_KEY, next.key, next.velocity));
		notes.erase(notes.begin());
		prev = next;
	}
}