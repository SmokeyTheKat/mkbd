#ifndef __MKBD_AUDIOPLAYER_HPP__
#define __MKBD_AUDIOPLAYER_HPP__

#include <mkbd/generator.hpp>

#include <vector>
#include <thread>
#include <mutex>
#include <SDL2/SDL.h>

class AudioPlayer {
	struct AudioSample {
		Generator generator;
		double freq;
		double gain;
		double t = 0;
		bool isFadingOut = false;
		double fadeOutTime = 0;
		inline AudioSample(Generator generator, double freq, double gain)
			: generator(generator), freq(freq / 441.0), gain(gain * 100.0) {};
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
	inline void sustainOn(void) { mSustain = true; };
	inline void sustainOff(void) { mSustain = false; };
	int addSample(Generator generator, double freq, double gain);
	void removeSample(double freq);
	void deleteSample(double freq);

private:
	static void audioCallback(void* vSelf, uint8_t* u8Buffer, int length);
	void fillAudioBuffer(int16_t* buffer, int length);
	SDL_AudioSpec initAudioSpec(void);

};

#endif