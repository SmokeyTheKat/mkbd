#ifndef __MKBD_MIDI_FILE_HPP__
#define __MKBD_MIDI_FILE_HPP__

#include <string>
#include <vector>

#include <mkbd/midi/keyboard.hpp>
#include <mkbd/midi/track.hpp>
#include <mkbd/utils.hpp>

namespace MidiReader {
	struct Header {
		uint32_t headerLength;
		uint16_t fileFormat;
		uint16_t trackCount;
		uint16_t tickInterval;
	};

	struct TrackHeader {
		uint32_t trackLength;
	};

	std::vector<MidiTrack> load(std::string path);
};

#endif