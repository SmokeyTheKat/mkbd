#include <mkbd/music.hpp>

#include <mkbd/midi/keyboard.hpp>
#include <mkbd/math.hpp>

#include <cmath>
#include <algorithm>
#include <iostream>

struct Chord {
	const char* name;
	const std::vector<int> notes;
};

static std::string checkChordOrientationName(std::vector<int> notes);

static constexpr const char* noteNames[12] = { "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" };

static const Chord chords[] = {
	{ "", { 0 } },

	{ " minor 2nd", { 0, 1 } },
	{ " major 2nd", { 0, 2 } },
	{ " minor 3rd", { 0, 3 } },
	{ " major 3rd", { 0, 4 } },
	{ " minor 3rd", { 0, 3 } },
	{ " perfect 4th", { 0, 5 } },
	{ " diminished 5th", { 0, 6 } },
	{ " perfect 5th", { 0, 7 } },
	{ " minor 6th", { 0, 8 } },
	{ " major 6th", { 0, 9 } },
	{ " minor 7th", { 0, 10 } },
	{ " major 7th", { 0, 11 } },
	{ " perfect octave", { 0, 12 } },
	{ " minor 9th", { 0, 13 } },
	{ " major 9th", { 0, 14 } },
	{ " minor 10th", { 0, 15 } },
	{ " major 10th", { 0, 16 } },
	{ " perfect 11th", { 0, 17 } },
	{ " diminished 12th", { 0, 18 } },
	{ " perfect 12th", { 0, 19 } },
	{ " minor 13th", { 0, 20 } },
	{ " major 13th", { 0, 21 } },
	{ " minor 14th", { 0, 22 } },
	{ " major 14th", { 0, 23 } },
	{ " perfect 2 octaves", { 0, 24 } },

	{ "maj", { 0, 4, 7, } },
	{ "m", { 0, 3, 7, } },
	{ "aug", { 0, 4, 8, } },
	{ "dim", { 0, 3, 6, } },

	{ "sus2", { 0, 2, 7, } },
	{ "sus4", { 0, 5, 7, } },
	{ "6sus4", { 0, 5, 7, 9, } },
	{ "6sus2", { 0, 2, 7, 9, } },
	{ "7sus4", { 0, 5, 7, 10, } },
	{ "7sus2", { 0, 2, 7, 10, } },
	{ "maj7sus4", { 0, 5, 7, 11, } },
	{ "maj7sus2", { 0, 2, 7, 11, } },

	{ "maj6", { 0, 4, 7, 9, } },
	{ "maj7", { 0, 4, 7, 11, } },
	{ "7", { 0, 4, 7, 10, } },
	{ "7b5", { 0, 4, 6, 10, } },
	{ "m(maj7)", { 0, 3, 7, 11, } },
	{ "m6", { 0, 3, 7, 9, } },
	{ "m7", { 0, 3, 7, 10, } },
	{ "m7#5", { 0, 3, 8, 10, } },
	{ "aug(maj7)", { 0, 4, 8, 11, } },
	{ "7#5", { 0, 4, 8, 10, } },
	{ "m7b5", { 0, 3, 6, 10, } },
	{ "m7b5", { 0, 3, 6, 10, } },
	{ "dim7", { 0, 3, 6, 9, } },


	{ "add9", { 0, 4, 7, 14, } },
	{ "madd9", { 0, 3, 7, 14, } },
	{ "6add9", { 0, 4, 7, 9, 14, } },
	{ "m6add9", { 0, 3, 7, 9, 14, } },
	{ "add11", { 0, 4, 7, 17, } },
	{ "madd11", { 0, 3, 7, 17, } },
	{ "7add11", { 0, 4, 7, 10, 17, } },
	{ "m7add11", { 0, 3, 7, 10, 17, } },
	{ "maj7add11", { 0, 4, 7, 11, 17, } },
	{ "m(maj7)add11", { 0, 3, 7, 11, 17, } },
	{ "7add13", { 0, 4, 7, 9, 10, } },
	{ "m7add13", { 0, 3, 7, 9, 10, } },
	{ "maj7add13", { 0, 4, 7, 9, 11, } },
	{ "m(maj7)add13", { 0, 3, 7, 9, 11, } },

	{ "maj9", { 0, 4, 7, 11, 14, } },
	{ "maj6/9", { 0, 4, 7, 9, 14, } },
	{ "m6/9", { 0, 3, 7, 9, 14, } },
	{ "9", { 0, 4, 7, 10, 14, } },
	{ "7b9", { 0, 4, 7, 10, 13, } },
	{ "7#9", { 0, 4, 7, 10, 15, } },
	{ "9#5b9", { 0, 4, 8, 10, 13, } },
	{ "m(maj9)", { 0, 3, 7, 11, 14, } },
	{ "m9", { 0, 3, 7, 10, 14, } },
	{ "m7b9", { 0, 3, 7, 10, 13, } },


	{ "maj11", { 0, 4, 7, 11, 14, 17, } },
	{ "11", { 0, 4, 7, 10, 14, 17, } },
	{ "9#11", { 0, 4, 7, 10, 14, 18, } },
	{ "m(maj11)", { 0, 3, 7, 11, 14, 17, } },
	{ "m11", { 0, 3, 7, 10, 14, 17, } },
	{ "aug(maj11)", { 0, 4, 8, 11, 14, 17, } },
	{ "aug11", { 0, 4, 8, 10, 14, 17, } },
	{ "m11b5", { 0, 3, 6, 10, 13, 17, } },
	{ "dim11", { 0, 3, 6, 9, 13, 16, } },

	{ "maj13", { 0, 4, 7, 11, 14, 17, 21, } },
	{ "13", { 0, 4, 7, 10, 14, 17, 21, } },
	{ "9b13", { 0, 4, 7, 10, 14, 20, } },
	{ "m(maj13)", { 0, 3, 7, 11, 14, 17, 21, } },
	{ "m13", { 0, 3, 7, 10, 14, 17, 21, } },
	{ "aug(maj13)", { 0, 4, 8, 11, 14, 17, 21, } },
	{ "aug13", { 0, 4, 8, 10, 14, 17, 21, } },
	{ "m13b5", { 0, 3, 6, 10, 14, 17, 21, } },

	{ 0 },
};

static std::string checkChordOrientationName(std::vector<int> notes) {
	std::sort(notes.begin(), notes.end());

	int front = notes.front();
	for (int& i : notes) {
		i -= front;
	}

	for (Chord c : chords) {
		if (c.notes == notes) {
			return std::string(Music::getNoteName(front)) + c.name;
		}
	}
	return "";
}


namespace Music {
	double tuning = 440;

	std::string getChordName(std::vector<int> notes) {
		if (notes.size() > 12)
			return "";

		int firstNote = notes.front();
		while (notes.front() < 112) {
			std::sort(notes.begin(), notes.end());
			std::string result = checkChordOrientationName(notes);
			if (result.size() > 0) {
				if (notes[0] != firstNote)
					return result + "/" + Music::getNoteName(firstNote);
				else return result;
			}
			notes[0] += 12;
		}
		return "";
	}

	double noteToFreq(int note) {
		return std::pow(std::pow(2.0, 1.0/12.0), note - 69.0) * tuning;
	}

	int freqToNote(double freq) {
		return std::round(12.0 * logn(freq / tuning, 2.0) + 69.0);
	}

	int getNoteId(int note) {
		return ((getNoteName(note)[0] - 'A' + 5) % 7) + (7 * (getNoteOctave(note)));
	}

	bool isBlackKey(int note) {
		return getNoteName(note).length() == 2;
	}

	std::string getNoteName(int note) {
		return noteNames[note % 12];
	}

	int getNoteOctave(int note) {
		return note / 12 - 1;
	}

	std::string getNoteFullName(int note) {
		return getNoteName(note) + std::to_string(getNoteOctave(note));
	}
};
