#include <mkbd/instruments.hpp>

std::vector<SampledInstrument*> instruments;

//SampledInstrument pianoInstrument("Piano", RESOURCE_DIR "/sounds/piano/", 21, 88 + 21);
//Generator _pianoGen = {
//    .waveform = pianoInstrument.getWaveform(),
//    .attack = LinearAttack<2>,
//    .release = Cutoff<3000>,
//    .fadeOut = LinearRelease<200>,
//};
//Generator* pianoGen = &_pianoGen;
//
//SampledInstrument piano2Instrument("Piano2", RESOURCE_DIR "/sounds/piano2/", 21, 88 + 21, SampleFormat::Numbers);
//Generator _piano2Gen {
//    .waveform = piano2Instrument.getWaveform(),
//    .release = Cutoff<3000>,
//    .fadeOut = LinearRelease<130>,
//};
//Generator* piano2Gen = &_piano2Gen;
//
//Generator _piano3Gen {
//    .waveform = [](double t, double freq, double vel) -> double {
//        return 0.5 * Waves::piano(t, freq);
//    },
//    .release = Cutoff<3000>,
//    .fadeOut = LinearRelease<100>,
//};
//Generator* piano3Gen = &_piano3Gen;
//
//Generator _synthGen {
//    .waveform = [](double t, double freq, double vel) -> double {
//        return Waves::synth(t, freq) * (vel * vel / 20);
//    },
//    .release = LinearRelease<800>,
//    .fadeOut = LinearRelease<100>,
//};
//Generator* synthGen = &_synthGen;
//
//Generator _brassGen {
//    .waveform = [](double t, double freq, double vel) -> double {
//        return Waves::multi(t, freq, 1.5, 2.0, 0.4);
//    },
//    .release = LinearRelease<9500>,
//    .fadeOut = LinearRelease<30>
//};
//Generator* brassGen = &_brassGen;
//
//Generator _phoneGen {
//    .waveform = [](double t, double freq, double vel) -> double {
//        return 0.25 * Waves::pulse(t, freq, 25) + 0.25 * Waves::sawtooth(t, freq) + 0.0001 * Waves::noise();
//    },
//    .fadeOut = LinearRelease<200>,
//};
//Generator* phoneGen = &_phoneGen;
//
//Generator _reedGen {
//    .waveform = [](double t, double freq, double vel) -> double {
//        return 0.5 * Waves::triangle(t, freq * 2) +
//               0.25 * Waves::sine(t, freq) +
//               0.25 * Waves::sawtooth(t, freq) +
//               0.04 * Waves::noise();
//    },
//    .fadeOut = LinearRelease<200>,
//};
//Generator* reedGen = &_reedGen;
//
//Generator _organGen {
//    .waveform = [](double t, double freq, double vel) -> double {
//        t += rmap(Waves::sine(t, 5.0), -1, 1, 0, 1.00);
//        return rmap(Waves::sine(t, 4.0), -1, 1, 1.0, 1) * (Waves::sine(t, freq * 1.0) +
//               0.5 * Waves::sine(t, freq * 2.0) +
//               0.25 * Waves::sine(t, freq * 4.0) +
//               0.125 * Waves::sine(t, freq * 8.0));
//    },
//    .fadeOut = LinearRelease<200>,
//};
//Generator* organGen = &_organGen;
//
Generator _metronomeGen {
	.waveform = [](double t, double freq, double vel) -> double {
		return 0.5 * Waves::multi(t, freq, 1.5, 2.0, 0.4);
	},
	.modifyers = {},
	.attack = Constant<1>,
	.release = LinearRelease<20>,
	.fadeOut = Constant<0>,
};
Generator* metronomeGen = &_metronomeGen;
