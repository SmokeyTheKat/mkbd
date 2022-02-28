#include <vector>
#include <thread>
#include <iostream>
#include <mutex>
#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>

class AudioPlayer {
	struct AudioSample {
		double freq;
		double gain;
		AudioSample(double freq, double gain)
			: freq(freq), gain(gain) {};
	};

	std::vector<AudioSample> samples;
	std::mutex mtx;
	std::thread audioThread;
	bool sampleChange = false;
	bool running = false;

public:
	void start(void) {
		audioThread = std::thread(&AudioPlayer::playSamples, this);
		running = true;
	}

	void stop(void) {
		mtx.lock();
		running = false;
		sampleChange = false;
		samples.clear();
		mtx.unlock();
		audioThread.join();
	}
	
	int addSample(double freq, double gain) {
		mtx.lock();

		sampleChange = true;

		samples.push_back(AudioSample(freq, gain));
		int id = samples.size() - 1;

		mtx.unlock();
		return id;
	}

	void removeSample(int id) {
		mtx.lock();

		sampleChange = true;

		samples.erase(samples.begin() + id);

		mtx.unlock();
	}

private:
	SDL_AudioSpec initAudioSpec(void) {
		SDL_AudioSpec audioSpec;
		SDL_zero(audioSpec);

		audioSpec.freq = 44100;
		audioSpec.format = AUDIO_S16SYS;
		audioSpec.channels = 1;
		audioSpec.samples = 1024;
		audioSpec.callback = 0;

		return audioSpec;
	}

	void playSamples(void) {
		SDL_Init(SDL_INIT_AUDIO);
		SDL_AudioSpec audioSpec = initAudioSpec();
		SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(0, 0, &audioSpec, 0, 0);
	
		SDL_PauseAudioDevice(audioDevice, 0);
	
		while (1) {
			mtx.lock();

			if (!running) {
				SDL_ClearQueuedAudio(audioDevice);
				mtx.unlock();
				break;
			}

			if (sampleChange) {
				SDL_ClearQueuedAudio(audioDevice);
				sampleChange = false;
			}

			float x = 0;
			for (int i = 0; i < audioSpec.freq * 5; i++) {
				x += .010f;
		
				int16_t sample = 0;
				for (AudioSample s : samples) {
					sample += sin(x * (2.0 * M_PI * s.freq * 100.0 / audioSpec.freq)) * s.gain * 1000.0;
				}
				
				SDL_QueueAudio(audioDevice, &sample, sizeof(int16_t));
			}

			mtx.unlock();

			while (running && !sampleChange && SDL_GetQueuedAudioSize(audioDevice) > sizeof(int16_t) * audioSpec.freq * 1) {
				SDL_Delay(10);
			}
		}
	
		SDL_CloseAudioDevice(audioDevice);
		SDL_Quit();
	}
};

int main(void) {
	AudioPlayer ap;
	ap.start();
	while (1) {
		std::cout << ":" << std::flush;
		double freq, gain = 3.0;
		std::cin >> freq;
		if (freq == 0) break;
		ap.addSample(freq, gain);
	}
	ap.stop();

	ap.start();
	while (1) {
		std::cout << "?" << std::flush;
		double freq, gain = 4.0;
		std::cin >> freq >> gain;
		if (freq == 0) break;
		ap.addSample(freq, gain);
	}
	ap.stop();
	return 0;
}