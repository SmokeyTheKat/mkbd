#include <mkbd/samplemanager.hpp>

#include <unordered_map>
#include <algorithm>
#include <iostream>

#define CONVERT_TO(bits) \
	if (bitRate == bits) { \
		double bitRatio = std::pow(2, 16) / std::pow(2, bitRate); \
		int##bits##_t* bufn = (int##bits##_t*)sample->buffer; \
		int16_t* buf16 = (int16_t*)sample->buffer; \
		for (int i = 0; i < sample->length; i++) \
			buf16[i] = (double)bufn[i] * bitRatio; \
	}

std::unordered_map<std::string, RawSample*> sampleMap;

static void fixPath(std::string& path) {
	std::replace(path.begin(), path.end(), '\\', '/');
	if (path.back() == '\r') path.pop_back();
}

namespace SampleManager {
	RawSample* loadSample(std::string path) {
		fixPath(path);

		if (sampleMap.count(path) > 0)
			return sampleMap[path];

		std::cout << path << "\n";

		RawSample* sample = new RawSample();

		sample->path = path;
		SDL_LoadWAV(path.c_str(), &sample->spec, (uint8_t**)&sample->buffer, &sample->length);

		if (sample->buffer == 0) exit(0);

		int bitRate = sample->getBitRate();
		sample->length /= bitRate / 8;

		CONVERT_TO(8);
		CONVERT_TO(32);
		CONVERT_TO(64);

		sampleMap[path] = sample;

		return sample;
	}

	void unloadSample(std::string path) {
		fixPath(path);

		if (sampleMap.count(path) == 0)
			return;

		unloadSample(sampleMap[path]);
	}

	void unloadSample(RawSample* sample) {
		auto it = std::find_if(sampleMap.begin(), sampleMap.end(), [sample](auto it) {
			return it.second == sample;
		});

		if (it == sampleMap.end())
			return;

		sampleMap.erase(it);

		SDL_FreeWAV((uint8_t*)sample->buffer);
		delete sample;
	}

};
