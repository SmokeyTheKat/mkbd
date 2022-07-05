#include <mkbd/instruments.hpp>

SampledInstrument pianoInstrument("Piano", RESOURCE_DIR "/sounds/piano/", 21, 88 + 21);
Generator pianoGen {
	.waveform = pianoInstrument.getWaveform(),
	.release = Cutoff<3000>,
	.fadeOut = LinearRelease<100>,
};

SampledInstrument piano2Instrument("Piano2", RESOURCE_DIR "/sounds/piano2/", 21, 88 + 21, SampleFormat::Numbers);
Generator piano2Gen {
	.waveform = piano2Instrument.getWaveform(),
	.release = Cutoff<3000>,
	.fadeOut = LinearRelease<130>,
};

Generator piano3Gen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * Waves::piano(t, freq);
	},
	.release = Cutoff<3000>,
	.fadeOut = LinearRelease<100>,
};

Generator synthGen {
	.waveform = Waves::synth,
	.release = LinearRelease<800>,
//    .release = LinearRelease<2000>,
	.fadeOut = LinearRelease<100>,
};

Generator brassGen {
	.waveform = [](double t, double freq) -> double {
		return Waves::multi(t, freq, 1.5, 2.0, 0.4);
	},
	.release = LinearRelease<9500>,
	.fadeOut = LinearRelease<30>
};

Generator phoneGen {
	.waveform = [](double t, double freq) -> double {
		return 0.25 * Waves::pulse(t, freq, 25) + 0.25 * Waves::sawtooth(t, freq) + 0.0001 * Waves::noise();
	},
	.fadeOut = LinearRelease<200>,
};

Generator reedGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * Waves::triangle(t, freq * 2) +
			   0.25 * Waves::sine(t, freq) +
			   0.25 * Waves::sawtooth(t, freq) +
			   0.04 * Waves::noise();
	},
	.fadeOut = LinearRelease<200>,
};

Generator organGen {
	.waveform = [](double t, double freq) -> double {
		return Waves::sine(t, freq * 1.0) +
			   0.5 * Waves::sine(t, freq * 2.0) +
			   0.25 * Waves::sine(t, freq * 4.0) +
			   0.125 * Waves::sine(t, freq * 8.0);
	},
	.fadeOut = LinearRelease<200>,
};

Generator metronomeGen {
	.waveform = [](double t, double freq) -> double {
		return 0.5 * Waves::multi(t, freq, 1.5, 2.0, 0.4);
	},
	.release = LinearRelease<20>,
//    .release = Cutoff<10>,
};
