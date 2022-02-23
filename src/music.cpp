#include <mkbd/music.hpp>

#include <mkbd/keyboard.hpp>

#include <algorithm>
#include <iostream>

struct Chord {
	const char* name;
	const std::vector<int> notes;
};

static constexpr const char* keyNames[12] = { "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab", "A", "Bb", "B" };

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
	{ " perfect 5th", { 0, 7 } },
	{ " minor 6th", { 0, 8 } },
	{ " major 6th", { 0, 9 } },
	{ " minor 7th", { 0, 10 } },
	{ " major 7th", { 0, 11 } },
	{ " perfect octave", { 0, 12 } },

	{ "maj", { 0, 4, 7, } },
	{ "m", { 0, 3, 7, } },
	{ "sus2", { 0, 2, 7, } },
	{ "sus4", { 0, 5, 7, } },
	{ "aug", { 0, 4, 8, } },
	{ "dim", { 0, 3, 6, } },

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
			return std::string(getKeyNameFromKey(front)) + c.name;
		}
	}
	return "";
}

std::string getChordName(std::vector<int> notes) {
	for (int& note : notes) {
		std::string result = checkChordOrientationName(notes);
		if (result.size() > 0)
			return result;
		note += 12;
	}
	return "Unknown";
}

const char* getKeyNameFromKey(byte key) {
	return keyNames[(key - 21 + 9) % 12];
}

int getKeyOctiveFromKey(byte key) {
	return (key - 21 + 9) / 12;
}
