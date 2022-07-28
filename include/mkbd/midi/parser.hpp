#ifndef __MKBD_MIDI_PARSER_HPP__
#define __MKBD_MIDI_PARSER_HPP__

#include <mkbd/utils.hpp>
#include <mkbd/midi/file.hpp>

class MidiParser {
	File mFile;

	MidiHeader mHeader;
	std::vector<MidiTrack> mTracks;

public:
	MidiParser(std::string path);

	MidiHeader& loadHeader(void);
	std::vector<MidiTrack>& loadTracks(void);
	MidiTrack& loadTrack(void);

private:
	void loadMidiEvent(MidiTrack& track, double beatCount);
	void loadMetaEvent(MidiTrack& track);
};

#endif