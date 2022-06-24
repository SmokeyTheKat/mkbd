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
		header.tickInterval = file.popNumber<uint16_t>(2) * 2;

		std::vector<MidiTrack> tracks;

		for (int t = 0; t < header.trackCount; t++) {
			std::cout << "=============================================\n";
			
			MidiReader::TrackHeader trackHeader;
			std::string tmagic = file.popString(4);
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
					std::string data = file.popString(metaLength);
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
			std::cout << ": " << events.size() << "\n";
			tracks.push_back(MidiTrack(events));
		}
		return tracks;
	};
};
