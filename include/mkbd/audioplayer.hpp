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
		inline AudioSample(Generator generator, double freq, double gain)
			: generator(generator), freq(freq / 441.0), gain(gain * 100.0), t(0) {};
	};

	std::vector<AudioSample> mSamples;
	std::mutex mMtx;
	std::thread mAudioThread;
	bool mSampleChange = false;
	bool mRunning = false;
	int mFreq = 44100;
	int mSampleRate = 22050;

public:
	void start(void);
	void stop(void);
	int addSample(Generator generator, double freq, double gain);
	void removeSample(double freq);

private:
	void playSamples(void);
	SDL_AudioSpec initAudioSpec(void);
	int getPlayedSampleCount(SDL_AudioDeviceID audioDevice);
	int getUnplayedSampleCount(SDL_AudioDeviceID audioDevice);
};

#endif