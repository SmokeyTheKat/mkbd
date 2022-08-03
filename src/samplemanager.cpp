#include <mkbd/samplemanager.hpp>

#include <unordered_map>
#include <algorithm>
#include <iostream>

std::unordered_map<std::string, RawSample*> sampleMap;

namespace SampleManager {
	RawSample* loadSample(std::string path) {
		std::replace(path.begin(), path.end(), '\\', '/');
		if (path.back() == '\r') {
			path.pop_back();
		}
		if (sampleMap.count(path) > 0) {
			return sampleMap[path];
		}
		std::cout << path << "\n";
		RawSample* sample = new RawSample();

		sample->path = path;
		SDL_LoadWAV(path.c_str(), &sample->spec, (uint8_t**)&sample->buffer, &sample->length);
		sample->length /= sizeof(SampleInt);

		sampleMap[path] = sample;

		return sample;
	}

	void unloadSample(std::string path) {
	}

	void unloadSample(RawSample* sample) {
	}

};
