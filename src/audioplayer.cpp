#include <mkbd/audioplayer.hpp>
#include <mkbd/math.hpp>
#include <mkbd/waves.hpp>
#include <mkbd/timer.hpp>

#include <cmath>
#include <iostream>

SDL_AudioSpec AudioPlayer::initAudioSpec(void) {
	SDL_AudioSpec audioSpec;
	SDL_zero(audioSpec);

	audioSpec.freq = mSampleRate;
	audioSpec.format = AUDIO_S16SYS;
	audioSpec.channels = 2;
	audioSpec.samples = 16;
	audioSpec.userdata = this;
	audioSpec.callback = AudioPlayer::audioCallback;

	return audioSpec;
}

void AudioPlayer::start(void) {
	SDL_AudioSpec audioSpec = initAudioSpec();
	mAudioDevice = SDL_OpenAudioDevice(0, 0, &audioSpec, 0, 0);
}

void AudioPlayer::unpause(void) {
	SDL_PauseAudioDevice(mAudioDevice, 0);
}

void AudioPlayer::pause(void) {
	SDL_PauseAudioDevice(mAudioDevice, 1);
}

void AudioPlayer::stop(void) {
	SDL_CloseAudioDevice(mAudioDevice);
	mSamples.clear();
}

void AudioPlayer::fillAudioBuffer(int16_t* buffer, int length) {
	int sampleCount = length / (sizeof(int16_t) * 2);

	mMtx.lock();

	for (int i = 0; i < sampleCount; i++) {
		std::vector<std::vector<AudioSample>::iterator> toDelete;

		int16_t sample = 0;
		for (auto& s : mSamples) {
			int16_t value = s.generator.sample(s.t, s.freq) * s.gain;

			if (s.fadeOutTime != 0) {
				if (mSustain && !s.isFadingOut) {
					s.fadeOutTime = s.t;
				} else {
					s.isFadingOut = true;
					value *= s.generator.fadeOut(s.t - s.fadeOutTime);
				}
			}

			if (s.generator.getModifyers(s.t) < 0.0000001) {
				for (auto it = mSamples.begin(); it != mSamples.end(); ++it) {
					if (it->freq == s.freq) {
						toDelete.push_back(it);
					}
				}
			}

			s.t += 0.01;
			sample += value;
		}

		*buffer++ = sample;
		*buffer++ = sample;

		for (auto it : toDelete) {
			mSamples.erase(it);
		}
	}

	mMtx.unlock();

}

void AudioPlayer::audioCallback(void* vSelf, uint8_t* buffer, int length) {
	AudioPlayer* self = (AudioPlayer*)vSelf;
	self->fillAudioBuffer((int16_t*)buffer, length);
}

void AudioPlayer::deleteSample(double freq) {
	std::vector<std::vector<AudioSample>::iterator> toDelete;

	for (auto it = mSamples.begin(); it != mSamples.end(); ++it) {
		if (isAbout(it->freq * 441.0, freq, 0.01)) {
			toDelete.push_back(it);
		}
	}

	for (auto it : toDelete) {
		mSamples.erase(it);
	}
}

int AudioPlayer::addSample(Generator generator, double freq, double gain) {
	mMtx.lock();

	deleteSample(freq);

	mSamples.push_back(AudioSample(generator, freq, gain));
	int id = mSamples.size() - 1;

	mMtx.unlock();
	return id;
}

void AudioPlayer::removeSample(double freq) {
	mMtx.lock();

	auto it = mSamples.begin();
	for (; it != mSamples.end(); ++it) {
		if (isAbout(it->freq * 441.0, freq, 0.01)) {
			break;
		}
	}

	if (it != mSamples.end()) {
		it->fadeOutTime = it->t;
	}

	mMtx.unlock();
}
