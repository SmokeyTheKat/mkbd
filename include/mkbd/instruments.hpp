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

static SampledInstrument piano2Instrument("Piano2", RESOURCE_DIR "/sounds/piano2/", 21, 88 + 21, SampleFormat::Numbers);
static Generator piano2Gen {
	.waveform = piano2Instrument.getWaveform(),
	.release = Cutoff<3000>,
	.fadeOut = LinearRelease<100>,
};

static Generator piano3Gen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * Waves::piano(t, freq);
	},
	.release = Cutoff<3000>,
	.fadeOut = LinearRelease<100>,
};

static Generator synthGen {
	.waveform = Waves::synth,
	.release = LinearRelease<2000>,
	.fadeOut = LinearRelease<100>,
};

static Generator brassGen {
	.waveform = [](double t, double freq) -> double {
		return Waves::multi(t, freq, 1.5, 2.0, 0.4);
	},
	.release = LinearRelease<9500>,
	.fadeOut = LinearRelease<100>
};

static Generator phoneGen {
	.waveform = [](double t, double freq) -> double {
		return 0.25 * Waves::pulse(t, freq, 25) + 0.25 * Waves::sawtooth(t, freq) + 0.0001 * Waves::noise();
	},
	.fadeOut = LinearRelease<200>,
};

static Generator reedGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * Waves::triangle(t, freq * 2) +
			   0.25 * Waves::sine(t, freq) +
			   0.25 * Waves::sawtooth(t, freq) +
			   0.04 * Waves::noise();
	},
	.fadeOut = LinearRelease<200>,
};

static Generator organGen {
	.waveform = [](double t, double freq) -> double {
		return Waves::sine(t, freq * 1.0) +
			   0.5 * Waves::sine(t, freq * 2.0) +
			   0.25 * Waves::sine(t, freq * 4.0) +
			   0.125 * Waves::sine(t, freq * 8.0);
	},
	.fadeOut = LinearRelease<200>,
};

static Generator metronomeGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * Waves::multi(t, freq, 1.5, 2.0, 0.4);
	},
	.release = LinearRelease<20>,
//    .release = Cutoff<10>,
};


#endif