#ifndef __MKBD_MUSIC_HPP__
#define __MKBD_MUSIC_HPP__

#include <string>
#include <vector>

#include <mkbd/utils.hpp>

std::string getChordName(std::vector<int> notes);
const char* getKeyNameFromKey(byte key);
int getKeyOctiveFromKey(byte key);

namespace Music {
	extern double tuning;
	double noteToFreq(int note);
	int freqToNote(double freq);
	std::string getNoteName(int note);
	int getNoteOctave(int note);
	std::string getNoteFullName(int note);
};

#endif