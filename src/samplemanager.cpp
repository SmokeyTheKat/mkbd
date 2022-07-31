#include <mkbd/samplemanager.hpp>

#include <unordered_map>

std::unordered_map<std::string, RawSample*> sampleMap;

namespace SampleManager {
	RawSample* loadSample(const std::string& path) {
		if (sampleMap.count(path) > 0) {
			return sampleMap[path];
		}
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
