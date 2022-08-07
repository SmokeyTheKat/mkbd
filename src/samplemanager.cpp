#include <mkbd/samplemanager.hpp>

#include <unordered_map>
#include <algorithm>
#include <iostream>

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
		sample->length /= sizeof(SampleInt);

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

		std::cout << "aaaaaaaaaaaaaaaaa\n";

		if (it == sampleMap.end())
			return;

		std::cout << "bbbbbbbbbbbbbbbbb\n";

		sampleMap.erase(it);

		SDL_FreeWAV((uint8_t*)sample->buffer);
		delete sample;
	}

};
