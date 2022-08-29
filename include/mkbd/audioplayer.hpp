#ifndef __MKBD_AUDIOPLAYER_HPP__
#define __MKBD_AUDIOPLAYER_HPP__

#include <mkbd/generator.hpp>
#include <mkbd/sample.hpp>

#include <cctype>
#include <vector>
#include <thread>
#include <mutex>
#include <SDL2/SDL.h>

constexpr double audioCutoff = 0.001;

static int gCount = 0;

class AudioPlayer {
	struct AudioSample {
	public:
		Generator* generator;
		int id;
		double freq;
		double gain;
		double vel = 127;
		double t = 0;
		bool isFadingOut = false;
		double fadeOutTime = -1;

		AudioSample(Generator* generator, double freq, double vel, double gain)
		: generator(generator), freq(freq), vel(vel), gain(gain), id(gCount) {
			gCount++;
		};

		double getFadeOutTime(void) { return t - fadeOutTime; };

		double fadeOut(void) {
			return generator->fadeOut(getFadeOutTime());
		};

		double sample(void) {
			return generator->sample(t, freq, vel) * gain;
		};

		bool isAudiable(void) {
			return generator->getModifyers(t) > audioCutoff && ((!isFadingOut && fadeOutTime < 0) || fadeOut() > audioCutoff);
		};
	};

	SDL_AudioDeviceID mAudioDevice;
	std::mutex mMtx;

	std::vector<AudioSample> mSamples;

	int mSampleRate = 44100;
	int mSampleSize = 128;

	bool mSustain = false;
	bool mIsPlaying = false;

public:
	void start(void);
	void pause(void);
	void unpause(void);
	void stop(void);
	void restart(void);

	void sustainOn(void) { mSustain = true; };
	void sustainOff(void) { mSustain = false; };

	void deleteSample(long id);

	void noteOn(Generator* generator, double freq, double vel, double gain);
	void noteOff(double freq);

	void setSampleSize(int sampleSize) { mSampleSize = sampleSize; };
	int getSampleSize(void) { return mSampleSize; };

private:
	SDL_AudioSpec initAudioSpec(void);

	static void audioCallback(void* vSelf, uint8_t* u8Buffer, int length);
	void fillAudioBuffer(int16_t* buffer, int length);

	double generateSample(AudioSample& s);

	void removeInaudiableSamples(void);
};

#endif