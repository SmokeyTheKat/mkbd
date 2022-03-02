#include <mkbd/player.hpp>

#include <mkbd/timer.hpp>

#include <unistd.h>

KeyboardPlayer::KeyboardPlayer(Keyboard* keyboard, std::vector<Key> notes, int bpm)
: mKeyboard(keyboard), mNotes(notes), mBpm(bpm) {}

void KeyboardPlayer::play(Keyboard* keyboard) {
	Timer timer;
	timer.start();
	Key prev = mNotes.front();
	while (mNotes.size() > 0) {
		Key next = mNotes.front();
		while (timer.now() < next.time) usleep(100);
		keyboard->sendMessage(KeyboardMessage(3, KBD_MSG_KEY, next.key, next.velocity));
		mNotes.erase(mNotes.begin());
		prev = next;
	}
}
