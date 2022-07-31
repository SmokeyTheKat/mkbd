#ifndef __MKBD_SAMPLEDINSTRUMENTS_HPP__
#define __MKBD_SAMPLEDINSTRUMENTS_HPP__

#include <mkbd/generator.hpp>
#include <mkbd/sample.hpp>

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

	std::vector<std::vector<Sample>> mSampleGroups;

public:
	SampledInstrument(const std::string& name, const std::string& path, int low, int high, SampleFormat format = SampleFormat::Names);

	Waveform getWaveform(void)  {
		return std::bind(&SampledInstrument::waveform, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	};

	void load(void);
	void unload(void);

	void setVolume(double volume) { mVolume = volume; };

private:
	int getClosestNoteTo(int to);
	double waveform(double t, double freq, double vel);
};

#endif