#ifndef __MKBD_SAMPLEDINSTRUMENTS_HPP__
#define __MKBD_SAMPLEDINSTRUMENTS_HPP__

#include <mkbd/generator.hpp>

#include <SDL2/SDL.h>

#include <string>
#include <vector>

class SampledInstrument {
	std::string mSamplePath;
	std::string mName;
	int mLowestNote;
	int mHighestNote;

	struct NoteSample {
		std::string path;
		SDL_AudioSpec spec;
		uint32_t length;
		int16_t* buffer;
	};

	std::vector<NoteSample> mSamples;

public:
	SampledInstrument(const std::string& name, const std::string& path, int low, int high);

	inline Waveform getWaveform(void)  {
		return std::bind(&SampledInstrument::waveform, this, std::placeholders::_1, std::placeholders::_2);
	};

	void load(void);
	void unload(void);

private:
	double waveform(double t, double freq);
};

#endif