#include <mkbd/instruments.hpp>

#include <mkbd/music.hpp>

#include <limits>

SampledInstrument::SampledInstrument(const std::string& name, const std::string& path, int low, int high)
: mName(name), mSamplePath(path), mLowestNote(low), mHighestNote(high) {
	mSamples.resize(mHighestNote);
};

double SampledInstrument::waveform(double t, double freq) {
	int note = Music::freqToNote(freq);

	if (note < mLowestNote || note >= mHighestNote)
		return 0;

	NoteSample& ns = mSamples[note];

	int idx = (int)(t * 100.0) * ns.spec.channels;

	if (idx > ns.length) 
		return 0;

	return 0.5 * (double)(ns.buffer[idx] + ns.buffer[idx+1]) / 2000.0;
}

void SampledInstrument::load(void) {
	for (int i = mLowestNote; i < mSamples.size() && i < mHighestNote; i++) {
		NoteSample& ns = mSamples[i];

		if (ns.path.length() == 0)
			ns.path = mSamplePath + "ff." + Music::getNoteFullName(i) + ".wav";

		SDL_LoadWAV(ns.path.c_str(), &ns.spec, (uint8_t**)&ns.buffer, &ns.length);
		ns.length /= sizeof(int16_t);
	}
}

void SampledInstrument::unload(void) {
	for (int i = mLowestNote; i < mSamples.size() && i < mHighestNote; i++) {
		const NoteSample& ns = mSamples[i];
		SDL_FreeWAV((uint8_t*)ns.buffer);
	}
	
}
