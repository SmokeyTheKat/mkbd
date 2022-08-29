#ifndef __MKBD_SAMPLE_HPP__
#define __MKBD_SAMPLE_HPP__

#include <mkbd/arrayview.hpp>

#include <string>

#include <SDL2/SDL.h>

typedef int16_t SampleInt;

struct RawSample {
	std::string path;
	SDL_AudioSpec spec;
	int mKeyCenter;
	uint32_t length = 0;
	SampleInt* buffer = 0;

	int getBitRate(void) { return spec.format & SDL_AUDIO_MASK_BITSIZE; };
};


class Sample {
	RawSample* mRawSample = 0;
	ArrayView<SampleInt> mData;

	int mDistanceFromKeyCenter = 0;
	double mLowVel = 0;
	double mHighVel = 127;
	double mPitchOffset = 0;
	double mVolume = 1;

public:
	Sample(void) {};
	Sample(RawSample* rawSample);

	int length(void) { return mData.length(); };
	int getChannelCount(void) { return mRawSample->spec.channels; };
	int getBitRate(void) { return mRawSample->spec.format & SDL_AUDIO_MASK_BITSIZE; };
	RawSample* getRawSample(void) { return mRawSample; };
	double getShiftFreqRatio(void);
	double getOffsetFreqRatio(void);
	bool isVelocityInRange(double vel) { return vel >= mLowVel && vel <= mHighVel; };

	void setDistanceFromKeyCenter(int distance) { mDistanceFromKeyCenter = distance; };
	void setVolume(double volume) { mVolume = volume; };
	void setPitchOffset(double pitchOffset) { mPitchOffset = pitchOffset; };
	void setVelocityRange(double low, double high) {
		mLowVel = low;
		mHighVel = high;
	};
	void setRange(int start, int end) {
		if (getBitRate() > 0) {
			mData = ArrayView<SampleInt>(mRawSample->buffer, start * 16 / getBitRate(), end * 16 / getBitRate());
		} else {
			mData = ArrayView<SampleInt>(mRawSample->buffer, start, end);
		}
	};

	bool hasPitchOffset(void) { return mPitchOffset != 0; };

	SampleInt get(int idx) { return mData[idx]; };
	double get(double idx);

	SampleInt operator[](int idx) { return get(idx); };
	double operator[](double idx) { return get(idx); };
};

#endif