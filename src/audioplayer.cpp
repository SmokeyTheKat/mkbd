#include <mkbd/audioplayer.hpp>
#include <mkbd/math.hpp>
#include <mkbd/waves.hpp>
#include <mkbd/timer.hpp>

#include <cmath>
#include <iostream>

void AudioPlayer::start(void) {
	mAudioThread = std::thread(&AudioPlayer::playSamples, this);
	mRunning = true;
}

void AudioPlayer::stop(void) {
	mMtx.lock();
	mRunning = false;
	mSampleChange = false;
	mSamples.clear();
	mMtx.unlock();
	mAudioThread.join();
}

int AudioPlayer::addSample(double freq, double gain) {
	mMtx.lock();

	mSampleChange = true;

	mSamples.push_back(AudioSample(freq, gain));
	int id = mSamples.size() - 1;

	std::cout << "add: " << mSamples.size() << " t: " << mSamples[id].t << "\n";

	mMtx.unlock();
	return id;
}

void AudioPlayer::removeSample(double freq) {
	mMtx.lock();

	mSampleChange = true;

	bool found = true;
	int i;
	for (i = 0; i < mSamples.size(); i++) {
		if (isAbout(mSamples[i].freq * 441.0, freq, 0.01)) {
			found = true;
			break;
		}
	}
	if (found) {
		mSamples.erase(mSamples.begin() + i);
	}

	mMtx.unlock();
}

SDL_AudioSpec AudioPlayer::initAudioSpec(void) {
	SDL_AudioSpec audioSpec;
	SDL_zero(audioSpec);

	audioSpec.freq = mFreq;
	audioSpec.format = AUDIO_S16SYS;
	audioSpec.channels = 1;
	audioSpec.samples = 1024;
	audioSpec.callback = 0;

	return audioSpec;
}

int AudioPlayer::getPlayedSampleCount(SDL_AudioDeviceID audioDevice) {
	int sampleCount = mSampleRate * mSamples.size();
	return sampleCount * sizeof(uint16_t) - SDL_GetQueuedAudioSize(audioDevice) / sizeof(uint16_t);
}
int AudioPlayer::getUnplayedSampleCount(SDL_AudioDeviceID audioDevice) {
	int sampleCount = mSampleRate * mSamples.size();
	int pc = sampleCount * sizeof(uint16_t) - SDL_GetQueuedAudioSize(audioDevice) / sizeof(uint16_t); 
	return sampleCount * sizeof(uint16_t) - pc;
}

double damp(double t) {
//    std::cout << t << "\n";
	double a = 1200.0;
	double val = (-1.0 / a) * (t - a);
	return (val < 0) ? 0 : val;
}

double dampOut(double t) {
	double a = 900.0;
	double val = (-1.0 / a) * (t - a);
	return (val < 0) ? 0 : val;
}

void AudioPlayer::playSamples(void) {
	SDL_Init(SDL_INIT_AUDIO);
	SDL_AudioSpec audioSpec = initAudioSpec();
	SDL_AudioDeviceID audioDevice = SDL_OpenAudioDevice(0, 0, &audioSpec, 0, 0);

	SDL_PauseAudioDevice(audioDevice, 0);

	while (1) {
		mMtx.lock();

		if (!mRunning) {
			SDL_ClearQueuedAudio(audioDevice);
			mMtx.unlock();
			break;
		}

		int sampleCount = mSampleRate * mSamples.size();

		for (auto& s : mSamples) {
//            double unplayedSamples = (double)getPlayedSampleCount(audioDevice) / ((double)mSamples.size());
			double unplayedSamples = (double)getUnplayedSampleCount(audioDevice) / ((double)mSamples.size());
			s.t -= unplayedSamples * 0.01;
			if (s.t < 0) s.t = 0;
		}

		if (mSampleChange) {
			mSampleChange = false;
		}

		SDL_ClearQueuedAudio(audioDevice);

		for (int i = 0; i < mSampleRate; i++) {
			int16_t sample = 0;
			for (int j = 0; j < mSamples.size(); j++) {
				auto& s = mSamples[j];
				s.t += 0.01;
				double value = 0;

				value += (0.5 * (Waves::sine(s.t, s.freq * 1.0) +
							  0.5 * Waves::sine(s.t, s.freq * 2.0) +
							  0.25 * Waves::sine(s.t, s.freq * 4.0) +
							  0.125 * Waves::sine(s.t, s.freq * 8.0))) * s.gain * damp(s.t);

				sample += value;
			}
			
			SDL_QueueAudio(audioDevice, &sample, sizeof(int16_t));
		}

		mMtx.unlock();

		while (mRunning && !mSampleChange && getUnplayedSampleCount(audioDevice) > 0) {
			SDL_Delay(10);
		}
	}

	SDL_CloseAudioDevice(audioDevice);
	SDL_Quit();
}