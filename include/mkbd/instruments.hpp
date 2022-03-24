#ifndef __MKBD_INSTRUMENTS_HPP__
#define __MKBD_INSTRUMENTS_HPP__

#include <mkbd/generator.hpp>
#include <mkbd/sampledinstruments.hpp>

static SampledInstrument pianoInstrument("Piano", RESOURCE_DIR "/sounds/piano/", 21, 88 + 21);
static Generator pianoGen {
	.waveform = pianoInstrument.getWaveform(),
	.release = Cutoff<3000>,
	.fadeOut = LinearRelease<100>,
};

static Generator brassGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * Waves::multi(t, freq, 1.5, 2.0, 0.4);
	},
	.release = LinearRelease<9500>,
	.fadeOut = LinearRelease<100>
};

static Generator phoneGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * (Waves::pulse(t, freq, 25) + 0.25 * Waves::sawtooth(t, freq) + 0.0001 * Waves::noise());
	},
	.fadeOut = LinearRelease<200>,
};

static Generator reedGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * (Waves::triangle(t, freq * 2) + 0.5 * Waves::sine(t, freq) + 0.25 * Waves::sawtooth(t, freq) + 0.04 * Waves::noise());
	},
	.fadeOut = LinearRelease<200>,
};

static Generator organGen {
	.waveform = [](double t, double freq) -> double {
		return (0.5 * (Waves::sine(t, freq * 1.0) +
					  0.5 * Waves::sine(t, freq * 2.0) +
					  0.25 * Waves::sine(t, freq * 4.0) +
					  0.125 * Waves::sine(t, freq * 8.0)));
	},
	.fadeOut = LinearRelease<200>,
};


#endif