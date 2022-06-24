#include <mkbd/midi/file.hpp>

#include <iostream>
#include <string>
#include <vector>

static int getEventLength(int event) {
	switch (event) {
		case 0x8: {
			return 3;
		} break;
		case 0x9: {
			return 3;
		} break;
		case 0xa: {
			return 3;
		} break;
		case 0xb: {
			return 3;
		} break;
		case 0xc: {
			return 2;
		} break;
		case 0xd: {
			return 2;
		} break;
		case 0xe: {
			return 3;
		} break;
	}
	return 2;
}

namespace MidiReader {
	std::vector<MidiTrack> load(std::string path) {
		File file(path);
		file.open();
		int ptr = 0;
		MidiReader::Header header;
		std::string hmagic = file.popString(4);
		if (hmagic != "MThd") {
			std::cout << "ERROR\n";
			return {};
		}

		header.headerLength = file.popNumber<uint32_t>(4);
		header.fileFormat = file.popNumber<uint16_t>(2);
		header.trackCount = file.popNumber<uint16_t>(2);
		header.tickInterval = file.popNumber<uint16_t>(2);

		std::vector<MidiTrack> tracks;

		for (int t = 0; t < header.trackCount; t++) {
			MidiReader::TrackHeader trackHeader;
			std::string tmagic = file.popString(4);
			std::cout << "====================\n";
			if (tmagic != "MTrk") {
				std::cout << "ERROR\n";
				return {};
			}
			trackHeader.trackLength = file.popNumber<uint32_t>(4);
			std::vector<MidiEvent> events;
			int end = file.getPosition() + trackHeader.trackLength;
			while (file.getPosition() < end) {
				uintmax_t deltaTime = file.popVarInt();
				if (file.front() == 0xff) {
					file.pop(1);
					byte metaEvent = file.popNumber<byte>(1);
					int metaLength = file.popNumber<byte>(1);
					if (metaEvent == 0x04) {
						std::string data = file.popString(metaLength);
						std::cout << "Instrument: " << data << "\n";
					} else if (metaEvent == 0x03) {
						std::string data = file.popString(metaLength);
						std::cout << "Track Name: " << data << "\n";
					} else if (metaEvent == 0x02) {
						std::string data = file.popString(metaLength);
						std::cout << "Copyright: " << data << "\n";
					} else if (metaEvent == 0x01) {
						std::string data = file.popString(metaLength);
						std::cout << "Text: " << data << "\n";
					} else if (metaEvent == 0x05) {
						std::string data = file.popString(metaLength);
						std::cout << "Lyric: " << data << "\n";
					} else if (metaEvent == 0x06) {
						std::string data = file.popString(metaLength);
						std::cout << "Text Marker: " << data << "\n";
					} else if (metaEvent == 0x07) {
						std::string data = file.popString(metaLength);
						std::cout << "Text Cue: " << data << "\n";
					} else if (metaEvent == 0x54) {
						std::string data = file.popString(metaLength);
						std::cout << "SMPTE" << "\n";
					} else if (metaEvent == 0x59) {
						char sf = file.popNumber(1);
						char mi = file.popNumber(1);
						std::string mm = "???";
						std::string key = "???";
						if (sf >= 0) {
							key = Music::circleOfFifths[sf];
						} else {
							key = Music::circleOfFourths[-sf];
						}
						if (mi == 0) {
							mm = "major";
						} else if (mi == 1) {
							mm = "minor";
						}
						std::cout << "Set Key Signature: " << key << " " << mm << "\n";
					} else if (metaEvent == 0x58) {
						int n = file.popNumber(1);
						int d = std::pow(2, file.popNumber(1));
						file.pop(2);
						std::cout << "Set Time Signature: " << n << "/" << d << "\n";
					} else if (metaEvent == 0x51) {
						double us = file.popNumber<uint32_t>(metaLength);
						double bpm = 60000000.0 / us;
						std::cout << "Set Tempo: " << bpm << "\n";
					} else {
						std::string data = file.popString(metaLength);
//                        std::cout << data << "\n";
					}
					continue;
				}
				if (file.front() < 0x80) {
					int eventLength = events.back().length() - 1;
					MidiEvent e(events.back()[0], file.pop(eventLength), (double)deltaTime / (double)header.tickInterval);
					events.push_back(MidiEvent(e));
				} else {
					int eventLength = getEventLength(file.front() / 16);
					MidiEvent e(file.pop(eventLength), (double)deltaTime / (double)header.tickInterval);
					events.push_back(MidiEvent(e));
				}
			}
			tracks.push_back(MidiTrack(events));
		}
		return tracks;
	};
};
