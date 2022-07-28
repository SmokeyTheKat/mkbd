#include <mkbd/midi/file.hpp>

#include <mkbd/midi/parser.hpp>

#include <iostream>
#include <string>
#include <vector>

void MidiFile::load(std::string path) {
	filePath = path;

	MidiParser parser(path);
	MidiHeader header = parser.loadHeader();
	tracks = parser.loadTracks();
};
