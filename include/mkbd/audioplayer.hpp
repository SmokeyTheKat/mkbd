#ifndef __MKBD_AUDIOPLAYER_HPP__
#define __MKBD_AUDIOPLAYER_HPP__

#include <vector>
#include <thread>
#include <mutex>
#include <SDL2/SDL.h>

class AudioPlayer {
	struct AudioSample {
		double freq;
		double gain;
		double t = 0;
		double et = 0;
		inline AudioSample(double freq, double gain)
			: freq(freq / 441.0), gain(gain * 100.0), t(0) {};
	};

	std::vector<AudioSample> samples;
	std::mutex mtx;
	std::thread audioThread;
	bool sampleChange = false;
	bool running = false;

public:
	void start(void);
	void stop(void);
	int addSample(double freq, double gain);
	void removeSample(double freq);

private:
	SDL_AudioSpec initAudioSpec(void);
	void playSamples(void);
};

#endif