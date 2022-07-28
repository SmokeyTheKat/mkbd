#include <mkbd/midi/parser.hpp>

static int getEventLength(int event);

MidiParser::MidiParser(std::string path) 
: mFile(path) {
	mFile.open();
}

MidiHeader& MidiParser::loadHeader(void) {
	std::string magic = mFile.popString(4);
	uint32_t headerLength = mFile.popNumber<uint32_t>(4);
	mHeader.format = (MidiFileFormat)mFile.popNumber<uint16_t>(2);
	mHeader.trackCount = mFile.popNumber<uint16_t>(2);
	mHeader.ticksPerBeat = mFile.popNumber<uint16_t>(2);

	return mHeader;
}

void MidiParser::loadMidiEvent(MidiTrack& track, double beatCount) {
	byte eventByte;
	int dataLength;
	if (mFile.front() < 0x80) {
		eventByte = track.events.back()[0];
		dataLength = track.events.back().length() - 1;
	} else {
		dataLength = getEventLength(mFile.front() / 16) - 1;
		eventByte = mFile.popByte();
	}
	track.events.push_back(MidiEvent(eventByte, mFile.pop(dataLength), beatCount));
}

void MidiParser::loadMetaEvent(MidiTrack& track) {
	mFile.pop(1);
	byte metaEvent = mFile.popByte();
	int metaLength = mFile.popByte();
	switch (metaEvent) {
		case 0x04: {
			std::string inst = mFile.popString(metaLength);
			std::cout << "Instrument: " << inst << "\n";
			track.instrument = inst;
		} break;
		case 0x03: {
			std::string name = mFile.popString(metaLength);
			std::cout << "Track Name: " << name << "\n";
			track.name = name;
		} break;
		case 0x02: {
			std::string data = mFile.popString(metaLength);
			std::cout << "Copyright: " << data << "\n";
		} break;
		case 0x01: {
			std::string data = mFile.popString(metaLength);
			std::cout << "Text: " << data << "\n";
		} break;
		case 0x05: {
			std::string data = mFile.popString(metaLength);
			std::cout << "Lyric: " << data << "\n";
		} break;
		case 0x06: {
			std::string data = mFile.popString(metaLength);
			std::cout << "Text Marker: " << data << "\n";
		} break;
		case 0x07: {
			std::string data = mFile.popString(metaLength);
			std::cout << "Text Cue: " << data << "\n";
		} break;
		case 0x54: {
			std::string data = mFile.popString(metaLength);
			std::cout << "SMPTE" << "\n";
		} break;
		case 0x59: {
			char sf = mFile.popNumber(1);
			char mi = mFile.popNumber(1);
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
		} break;
		case 0x58: {
			int n = mFile.popNumber(1);
			int d = std::pow(2, mFile.popNumber(1));
			mFile.pop(2);
			std::cout << "Set Time Signature: " << n << "/" << d << "\n";
		} break;
		case 0x51: {
			double us = mFile.popNumber<uint32_t>(metaLength);
			double bpm = 60000000.0 / us;
			std::cout << "Set Tempo: " << bpm << "\n";
		} break;
		default: {
			std::string data = mFile.popString(metaLength);
		} break;
	}
}


MidiTrack& MidiParser::loadTrack(void) {
	std::cout << "====================\n";
	std::string magic = mFile.popString(4);

	mTracks.push_back(MidiTrack());
	MidiTrack& track = mTracks.back();

	uint32_t trackByteCount = mFile.popNumber<uint32_t>(4);

	double totalBeatCount = 0;

	int end = mFile.getPosition() + trackByteCount;
	while (mFile.getPosition() < end) {
		uintmax_t deltaTime = mFile.popVarInt();
		double beatCount = (double)deltaTime / (double)mHeader.ticksPerBeat;
		totalBeatCount += beatCount;

		if (mFile.front() == 0xff) {
			loadMetaEvent(track);
		} else {
			loadMidiEvent(track, beatCount);
		}
	}

	return track;
}


std::vector<MidiTrack>& MidiParser::loadTracks(void) {
	for (int i = 0; i < mHeader.trackCount; i++) {
		loadTrack();
	}
	return mTracks;
}


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
