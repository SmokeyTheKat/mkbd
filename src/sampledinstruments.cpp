#include <mkbd/instruments.hpp>

#include <mkbd/music.hpp>
#include <mkbd/filemanager.hpp>
#include <mkbd/waves.hpp>

#include <limits>
#include <valarray>
#include <complex>

SampledInstrument::SampledInstrument(const std::string& name, const std::string& path, int low, int high, SampleFormat format)
: mName(name), mSamplePath(path), mLowestNote(low), mHighestNote(high), mFormat(format) {
	mVolume = 1;
	mSamples.resize(mHighestNote);
};

double SampledInstrument::waveform(double t, double freq) {
	int note = Music::freqToNote(freq);

	if (note < mLowestNote || note >= mHighestNote)
		return 0;

	NoteSample* ns = &mSamples[note];

	if (ns->shift != 0) {
		t *= Music::noteToFreq(note) / Music::noteToFreq(note + ns->shift);
		ns = &mSamples[note + ns->shift];
	}

	double pt = t * 100.0 * (double)ns->spec.channels;

	if (pt > ns->length) 
		return 0;

	int leftIdx = std::floor(pt)-1;
	int rightIdx = std::ceil(pt)+1;

	double leftSample = 0.5 * (double)(ns->buffer[leftIdx] + ns->buffer[leftIdx+1]) / 2000.0;
	double rightSample = 0.5 * (double)(ns->buffer[rightIdx] + ns->buffer[rightIdx+1]) / 2000.0;

	return rmap(pt, leftIdx, rightIdx, leftSample, rightSample) * mVolume;
}

void SampledInstrument::load(void) {
	for (int i = mLowestNote; i < mSamples.size() && i < mHighestNote; i++) {
		NoteSample& ns = mSamples[i];

		if (ns.path.length() == 0) {
			if (mFormat == SampleFormat::Names)
				ns.path = mSamplePath + "ff." + Music::getNoteFullName(i) + ".wav";
			else if (mFormat == SampleFormat::Numbers)
				ns.path = mSamplePath + std::to_string(i) + ".wav";
		}
		if (FileManager::fileExists(ns.path.c_str())) {
			SDL_LoadWAV(ns.path.c_str(), &ns.spec, (uint8_t**)&ns.buffer, &ns.length);
			ns.length /= sizeof(int16_t);
		}
	}
	for (int i = mLowestNote; i < mSamples.size() && i < mHighestNote; i++) {
		NoteSample& ns = mSamples[i];
		if (ns.length == 0) {
			ns.shift = getClosestNoteTo(i) - i;
		}
	}
}

int SampledInstrument::getClosestNoteTo(int to) {
	int clUp = 0;
	for (int i = to + 1; i < mSamples.size() && i < mHighestNote; i++) {
		NoteSample& ns = mSamples[i];
		if (ns.length > 0) {
			clUp = i;
			break;
		}
	}

	int clDown = 0;
	for (int i = to - 1; i >= 0; i--) {
		NoteSample& ns = mSamples[i];
		if (ns.length > 0) {
			clDown = i;
			break;
		}
	}

	if (to - clDown < clUp - to || clUp == 0) {
		return clDown;
	} else {
		return clUp;
	}
}

void SampledInstrument::unload(void) {
	for (int i = mLowestNote; i < mSamples.size() && i < mHighestNote; i++) {
		const NoteSample& ns = mSamples[i];
		SDL_FreeWAV((uint8_t*)ns.buffer);
	}
	
}
