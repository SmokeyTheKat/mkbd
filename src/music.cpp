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

//-------------------------------------------------------------------
	{ "maj",              { 0, 4, 7, } },

	{ "maj6",             { 0, 4, 7, 9, } },

	{ "maj6add9",         { 0, 2, 4, 7, 9, } },

	{ "maj7",             { 0, 4, 7, 11, } },
	{ "maj7",             { 0, 4, 11, } },

	{ "maj7#5",           { 0, 4, 8, 11, } },

	{ "maj7b5",           { 0, 4, 6, 11, } },

	{ "maj7#11",          { 0, 4, 6, 7, 11, } },

	{ "maj9",             { 0, 2, 4, 7, 11, } },
	{ "maj9",             { 0, 2, 4, 11, } },

	{ "maj9#5",           { 0, 2, 4, 8, 11, } },

	{ "maj9b5",           { 0, 2, 4, 6, 11, } },

	{ "maj9#11",          { 0, 2, 4, 6, 7, 11, } },

	{ "maj11",            { 0, 2, 4, 5, 7, 11, } },
	{ "maj11",            { 0, 2, 4, 5, 11, } },
	{ "maj11",            { 0, 4, 5, 7, 11, } },
	{ "maj11",            { 0, 4, 5, 11, } },

	{ "maj13",            { 0, 2, 4, 5, 7, 9, 11, } },
	{ "maj13",            { 0, 2, 4, 5, 9, 11, } },
	{ "maj13",            { 0, 4, 5, 7, 9, 11, } },
	{ "maj13",            { 0, 4, 5, 9, 11, } },
	{ "maj13",            { 0, 2, 4, 7, 9, 11, } },
	{ "maj13",            { 0, 2, 4, 9, 11, } },
	{ "maj13",            { 0, 4, 7, 9, 11, } },
	{ "maj13",            { 0, 4, 9, 11, } },

	{ "maj13#11",            { 0, 2, 4, 6, 7, 9, 11, } },
	{ "maj13#11",            { 0, 2, 4, 6, 9, 11, } },
	{ "maj13#11",            { 0, 4, 6, 7, 9, 11, } },
	{ "maj13#11",            { 0, 4, 6, 9, 11, } },

	{ "add9",             { 0, 2, 4, 7, } },

//-------------------------------------------------------------------
	{ "m",                { 0, 3, 7, } },

	{ "m6",               { 0, 3, 7, 9, } },

	{ "m6add9",               { 0, 2, 3, 7, 9, } },

	{ "m7",               { 0, 3, 7, 10, } },
	{ "m7",               { 0, 3, 10, } },

	{ "m7#5",             { 0, 3, 8, 10, } },

	{ "m7b9",             { 0, 1, 3, 7, 10, } },

	{ "m7b13",            { 0, 3, 7, 8, 10, } },

	{ "m9",               { 0, 2, 3, 7, 10, } },
	{ "m9",               { 0, 2, 3, 10, } },

	{ "m11",              { 0, 2, 3, 5, 7, 10, } },
	{ "m11",              { 0, 2, 3, 5, 10, } },
	{ "m11",              { 0, 3, 5, 7, 10, } },
	{ "m11",              { 0, 3, 5, 10, } },

	{ "m11b9",            { 0, 1, 3, 5, 7, 10, } },
	{ "m11b9",            { 0, 1, 3, 5, 10, } },

	{ "m11b13",           { 0, 2, 3, 5, 7, 8, 10, } },
	{ "m11b13",           { 0, 2, 3, 5, 8, 10, } },
	{ "m11b13",           { 0, 3, 5, 7, 8, 10, } },
	{ "m11b13",           { 0, 3, 5, 8, 10, } },

	{ "m11b13b9",         { 0, 1, 3, 5, 7, 8, 10, } },
	{ "m11b13b9",         { 0, 1, 3, 5, 8, 10, } },

	{ "m13",              { 0, 2, 3, 5, 7, 9, 10 } },
	{ "m13",              { 0, 2, 3, 5, 9, 10 } },
	{ "m13",              { 0, 3, 5, 7, 9, 10 } },
	{ "m13",              { 0, 3, 5, 9, 10 } },
	{ "m13",              { 0, 2, 3, 7, 9, 10} },
	{ "m13",              { 0, 2, 3, 9, 10 } },
	{ "m13",              { 0, 3, 7, 9, 10 } },
	{ "m13",              { 0, 3, 9, 10 } },

	{ "m13b9",            { 0, 1, 3, 5, 7, 9, 10 } },
	{ "m13b9",            { 0, 1, 3, 5, 9, 10 } },
	{ "m13b9",            { 0, 1, 3, 7, 9, 10} },
	{ "m13b9",            { 0, 1, 3, 9, 10 } },

	{ "madd9",            { 0, 2, 3, 7, } },

	{ "m7b5", { 0, 3, 6, 10, } },

//-------------------------------------------------------------------
	{ "aug",              { 0, 4, 8, } },

//-------------------------------------------------------------------
	{ "dim",              { 0, 3, 6, } },
	{ "dim7",             { 0, 3, 6, 9, } },
	{ "dim9",             { 0, 2, 3, 6, 9, } },
	{ "dim11",            { 0, 2, 3, 5, 6, 9, } },

//-------------------------------------------------------------------
	{ "sus2",             { 0, 2, 7, } },
	{ "sus4",             { 0, 5, 7, } },

//-------------------------------------------------------------------
	{ "7",                { 0, 4, 7, 10, } },
	{ "7",                { 0, 4, 10, } },

	{ "7b5",              { 0, 4, 6, 10, } },

	{ "7#5",              { 0, 4, 8, 10, } },

	{ "7b9",              { 0, 1, 4, 7, 10, } },
	{ "7b9",              { 0, 1, 4, 10, } },

	{ "7#9",              { 0, 3, 4, 7, 10, } },
	{ "7#9",              { 0, 3, 4, 10, } },

	{ "7b5b9",            { 0, 1, 4, 6, 10, } },

	{ "7b5#9",            { 0, 3, 4, 6, 10, } },

	{ "7#5#9",            { 0, 3, 4, 8, 10, } },

	{ "7#5b9",            { 0, 1, 4, 8, 10, } },

	{ "9",                { 0, 2, 4, 7, 10, } },
	{ "9",                { 0, 2, 4, 10, } },

	{ "9b5",              { 0, 2, 4, 6, 10, } },

	{ "9#5",              { 0, 2, 4, 8, 10, } },

	{ "11",               { 0, 2, 4, 5, 7, 10, } },
	{ "11",               { 0, 2, 4, 5, 10, } },
	{ "11",               { 0, 4, 5, 7, 10, } },
	{ "11",               { 0, 4, 5, 10, } },

	{ "13",               { 0, 2, 4, 5, 7, 9, 10, } },
	{ "13",               { 0, 2, 4, 5, 9, 10, } },
	{ "13",               { 0, 4, 5, 7, 9, 10, } },
	{ "13",               { 0, 4, 5, 9, 10, } },
	{ "13",               { 0, 2, 4, 7, 9, 10, } },
	{ "13",               { 0, 2, 4, 9, 10, } },
	{ "13",               { 0, 4, 7, 9, 10, } },
	{ "13",               { 0, 4, 9, 10, } },

	{ 0 },
};

static std::string checkChordOrientationName(std::vector<int> notes) {
	int front = notes.front();
	for (int& i : notes) {
		i = (i - front) % 12;
	}

	std::sort(notes.begin(), notes.end());

	notes.erase(std::unique(notes.begin(), notes.end()), notes.end());

	for (Chord c : chords) {
		if (c.notes == notes) {
			return c.name;
		}
	}
	return "";
}


namespace Music {
	double tuning = 440;

	std::string getChordName(std::vector<int> notes) {
		if (notes.size() > 12)
			return "";

		int front = notes.front();

		while (notes.front() < 112) {
			std::sort(notes.begin(), notes.end());
			std::string result = checkChordOrientationName(notes);
			if (result.size() > 0) {
				if (notes[0] != front)
					return Music::getNoteName(notes[0]) + result + "/" + Music::getNoteName(front);
				else return Music::getNoteName(notes[0]) + result;
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

	double centsToFreqRatio(double cents) {
		return std::pow(2, cents / 1200.0);
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
