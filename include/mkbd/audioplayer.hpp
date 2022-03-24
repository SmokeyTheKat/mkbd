#ifndef __MKBD_AUDIOPLAYER_HPP__
#define __MKBD_AUDIOPLAYER_HPP__

#include <mkbd/generator.hpp>

#include <vector>
#include <thread>
#include <mutex>
#include <SDL2/SDL.h>

constexpr double audioCutoff = 0.001;

class AudioPlayer {
	struct AudioSample {
	private:
		inline static long count = 0;

	public:
		Generator generator;
		long id;
		double freq;
		double gain;
		double t = 0;
		bool isFadingOut = false;
		double fadeOutTime = -1;

		AudioSample(Generator generator, double freq, double gain)
		: generator(generator), freq(freq), gain(gain * 100.0), id(count) {
			count++;
		};

		double getFadeOutTime(void) { return t - fadeOutTime; };

		double fadeOut(void) {
			return generator.fadeOut(getFadeOutTime());
		};

		double sample(void) {
			return generator.sample(t, freq) * gain;
		};

		bool isAudiable(void) {
			return generator.getModifyers(t) > audioCutoff && ((!isFadingOut && fadeOutTime < 0) || fadeOut() > audioCutoff);
		};

		static void resetCount(void) { count = 0; };
	};

	std::vector<AudioSample> mSamples;
	int mSampleRate = 44100;
	SDL_AudioDeviceID mAudioDevice;
	std::mutex mMtx;
	bool mSustain = false;

public:
	void start(void);
	void pause(void);
	void unpause(void);
	void stop(void);
	void restart(void);

	void sustainOn(void) { mSustain = true; };
	void sustainOff(void) { mSustain = false; };

	void deleteSample(long id);
	void noteOn(Generator generator, double freq, double gain);
	void noteOff(double freq);

private:
	SDL_AudioSpec initAudioSpec(void);

	static void audioCallback(void* vSelf, uint8_t* u8Buffer, int length);
	void fillAudioBuffer(int16_t* buffer, int length);

	int16_t generateSample(AudioSample& s);
};

#endif