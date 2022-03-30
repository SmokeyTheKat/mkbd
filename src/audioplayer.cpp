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
	audioSpec.channels = 1;
	audioSpec.samples = 128;
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

void AudioPlayer::restart(void) {
	stop();
	start();
}

int16_t AudioPlayer::generateSample(AudioSample& s) {
	double value = s.sample();
	if (s.fadeOutTime >= 0) {
		if (mSustain && !s.isFadingOut) {
			s.fadeOutTime = s.t;
		} else {
			s.isFadingOut = true;
			value *= s.fadeOut();
		}
	}
	return shrink<double, int16_t>(value);
}

void AudioPlayer::removeInaudiableSamples(void) {
	std::vector<long> toDelete;

	for (auto& s : mSamples) {
		if (!s.isAudiable())
			toDelete.push_back(s.id);
	}

	for (auto id : toDelete)
		deleteSample(id);
}

void AudioPlayer::fillAudioBuffer(int16_t* buffer, int length) {
	int sampleCount = length / (sizeof(int16_t));

	mMtx.lock();

	for (int i = 0; i < sampleCount; i++) {
		intmax_t sample = 0;
		for (auto& s : mSamples) {
			sample += generateSample(s);
			s.t += 0.01;
		}
		*buffer++ = shrink<intmax_t, int16_t>(sample);
	}

	removeInaudiableSamples();

	mMtx.unlock();
}

void AudioPlayer::audioCallback(void* vSelf, uint8_t* buffer, int length) {
	AudioPlayer* self = (AudioPlayer*)vSelf;
	self->fillAudioBuffer((int16_t*)buffer, length);
}

void AudioPlayer::deleteSample(long id) {
	mSamples.erase(
		std::remove_if(mSamples.begin(), mSamples.end(), [id](const AudioSample& s) {
			return s.id == id;
		}),
		mSamples.end()
	);

	if (mSamples.size() == 0)
		AudioSample::resetCount();
}

void AudioPlayer::noteOn(Generator generator, double freq, double gain) {
	mMtx.lock();

	mSamples.push_back(AudioSample(generator, freq, gain));

	mMtx.unlock();
}

void AudioPlayer::noteOff(double freq) {
	mMtx.lock();

	for (auto& s : mSamples) {
		if (isAbout(s.freq, freq, 0.01)) {
			if (!s.isFadingOut && s.fadeOutTime < 0) {
				s.fadeOutTime = s.t;
			}
		}
	}

	mMtx.unlock();
}
