#include <mkbd/audioplayer.hpp>
#include <mkbd/math.hpp>
#include <mkbd/waves.hpp>

#include <cmath>
#include <iostream>

void AudioPlayer::start(void) {
	audioThread = std::thread(&AudioPlayer::playSamples, this);
	running = true;
}

void AudioPlayer::stop(void) {
	mtx.lock();
	running = false;
	sampleChange = false;
	samples.clear();
	mtx.unlock();
	audioThread.join();
}

int AudioPlayer::addSample(double freq, double gain) {
	mtx.lock();

	sampleChange = true;

	samples.push_back(AudioSample(freq, gain));
	int id = samples.size() - 1;

	std::cout << "add: " << samples.size() << " t: " << samples[id].t << "\n";

	mtx.unlock();
	return id;
}

void AudioPlayer::removeSample(double freq) {
	mtx.lock();

	sampleChange = true;

	bool found = true;
	int i;
	for (i = 0; i < samples.size(); i++) {
		if (isAbout(samples[i].freq * 441.0, freq, 1)) {
			found = true;
			break;
		}
	}
	if (found) {
		AudioSample& as = samples[i];
		if (as.et == 0)
			as.et = as.t;
		std::cout << "et: " << as.et << " t: " << as.t << "\n";
	//    samples.erase(samples.begin() + i);
	}

	mtx.unlock();
}

SDL_AudioSpec AudioPlayer::initAudioSpec(void) {
	SDL_AudioSpec audioSpec;
	SDL_zero(audioSpec);

	audioSpec.freq = 44100;
	audioSpec.format = AUDIO_S16SYS;
	audioSpec.channels = 1;
	audioSpec.samples = 1024;
	audioSpec.callback = 0;

	return audioSpec;
}

double damp(double t) {
	double a = 900.0;
	double val = (-1.0 / a) * (t - a);
	return (val < 0) ? 0 : val;
}

double dampOut(double t) {
	double a = 900.0;
	double val = (-1.0 / a) * (t - a);
	return (val < 0) ? 0 : val;
}


SDL_Window *createWindow()
{
	return SDL_CreateWindow("Simple Tone Example",
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
							1600,
							900,
							SDL_WINDOW_SHOWN);
}


void AudioPlayer::playSamples(void) {
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

		for (int i = 0; i < audioSpec.freq / 3; i++) {
			int16_t sample = 0;
			for (int j = 0; j < samples.size(); j++) {
				auto& s = samples[j];
				s.t += 0.01;
				double value = 0;

//                value += (0.5 * (Waves::pulse(s.t, s.freq, 10) + 0.5 * (Waves::square(s.t, s.freq * 2.0)) + 0.0001 * Waves::noise())) * s.gain;
//                value += (0.5 * Waves::pulse(s.t, s.freq, 10)) * s.gain;// * damp(s.t);
//                value += (0.5 * Waves::square(s.t, s.freq)) * s.gain * damp(s.t);
//                value += (0.5 * Waves::piano2(s.t, s.freq)) * s.gain;// * damp(s.t);
//                value += (0.5 * Waves::sine(s.t, s.freq)) * s.gain;
//                value += (0.5 * (Waves::sine(s.t, s.freq * 2.0) + 0.0001 * Waves::noise())) * s.gain * damp(s.t);

				value += (0.5 * (Waves::sine(s.t, s.freq * 1.0) +
							  0.5 * Waves::sine(s.t, s.freq * 2.0) +
							  0.25 * Waves::sine(s.t, s.freq * 4.0) +
							  0.125 * Waves::sine(s.t, s.freq * 8.0))) * s.gain;// * damp(s.t);

				if (j == 0 && s.et != 0) {
					double dp = damp(s.t - s.et);
//                    std::cout << j << ": " << s.t << " " << s.et << "\n";
					value *= dp;
					if (dp < 0.01) {
						samples.erase(samples.begin() + j, samples.begin() + j + 1);
						j--;
						std::cout << "delete: " << dp << " " << samples.size() << "\n";
						continue;
					}
				}
				sample += value;
			}
			
			SDL_QueueAudio(audioDevice, &sample, sizeof(int16_t));
		}

		mtx.unlock();

		while (running && !sampleChange && SDL_GetQueuedAudioSize(audioDevice) > sizeof(int16_t) * audioSpec.freq / 2) {
			SDL_Delay(10);
		}
	}

	SDL_CloseAudioDevice(audioDevice);
	SDL_Quit();
}