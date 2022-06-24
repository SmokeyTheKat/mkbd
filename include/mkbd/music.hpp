#ifndef __MKBD_MUSIC_HPP__
#define __MKBD_MUSIC_HPP__

#include <string>
#include <vector>

#include <mkbd/utils.hpp>

namespace Music {
	extern double tuning;

	static constexpr const char* circleOfFifths[] = {
		"C", "G", "D", "A", "E", "B", "F#", "Db", "Ab", "Eb", "Bb", "F"
	};
	static constexpr const char* circleOfFourths[] = {
		"C", "F", "Bb", "Eb", "Ab", "Db", "F#", "B", "E", "A", "D", "G"
	};

	struct Note {
		byte note;
		byte velocity;
		double time;

		Note(void)
		: Note(0, 0, 0) {};
		Note(byte note)
		: Note(note, 0, 0) {};
		Note(byte note, byte velocity)
		: Note(note, velocity, 0) {};
		Note(byte note, byte velocity, double time)
		: note(note), velocity(velocity), time(time) {};

		operator bool() { return !(note == 0 && velocity == 0 && time == 0); };

		static Note None(void) { return Note(); };
	};

	std::string getChordName(std::vector<int> notes);

	double noteToFreq(int note);
	int freqToNote(double freq);

	int getNoteId(int note);
	bool isBlackKey(int note);
	std::string getNoteName(int note);
	int getNoteOctave(int note);
	std::string getNoteFullName(int note);
};

#endif