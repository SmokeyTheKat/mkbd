#ifndef __MKBD_SAMPLEDINSTRUMENTS_HPP__
#define __MKBD_SAMPLEDINSTRUMENTS_HPP__

#include <mkbd/generator.hpp>

#include <SDL2/SDL.h>

#include <string>
#include <vector>

enum class SampleFormat {
	Names,
	Numbers
};

class SampledInstrument {
	std::string mSamplePath;
	std::string mName;
	SampleFormat mFormat;

	double mVolume = 1.0;

	int mLowestNote;
	int mHighestNote;

	struct NoteSample {
		int shift = 0;
		std::string path;
		SDL_AudioSpec spec;
		uint32_t length = 0;
		int16_t* buffer = 0;
	};

	std::vector<NoteSample> mSamples;

public:
	SampledInstrument(const std::string& name, const std::string& path, int low, int high, SampleFormat format = SampleFormat::Names);

	Waveform getWaveform(void)  {
		return std::bind(&SampledInstrument::waveform, this, std::placeholders::_1, std::placeholders::_2);
	};

	void load(void);
	void unload(void);

	void setVolume(double volume) { mVolume = volume; };

private:
	int getClosestNoteTo(int to);
	double waveform(double t, double freq);
};

#endif