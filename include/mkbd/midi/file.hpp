#ifndef __MKBD_MIDI_FILE_HPP__
#define __MKBD_MIDI_FILE_HPP__

#include <string>
#include <vector>

#include <mkbd/midi/keyboard.hpp>
#include <mkbd/midi/track.hpp>
#include <mkbd/utils.hpp>

enum class MidiFileFormat {
	Single,
	Synchronous,
	Asynchronous
};

struct MidiHeader {
	MidiFileFormat format;
	int trackCount;
	int ticksPerBeat;
};

struct MidiFile {
	std::string filePath;
	std::vector<MidiTrack> tracks;

	MidiFileFormat format;
	int ticksPerBeat;

	MidiFile(void) {};
	MidiFile(std::string path) { load(path); };
	void load(std::string path);
	bool isEmpty(void) { return tracks.size() == 0; };

private:
};

#endif