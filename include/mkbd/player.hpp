#ifndef __MKBD_PLAYER_HPP__
#define __MKBD_PLAYER_HPP__

#include <mkbd/keyboard.hpp>

#include <vector>

class KeyboardPlayer {
	Keyboard* mKeyboard;
	std::vector<Key> mNotes;
	int mBpm;

public:
	KeyboardPlayer(Keyboard* keybaord, std::vector<Key> notes, int bpm);

	void play(Keyboard* keyboard);
};

#endif