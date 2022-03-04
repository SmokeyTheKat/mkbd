#ifndef __MKBD_GENERATOR_HPP__
#define __MKBD_GENERATOR_HPP__

#include <mkbd/waves.hpp>

#include <functional>

class Modifyer {
public:
	virtual double operator()(double t) = 0;
};

class ModifyerFunction : public Modifyer {
public:
	std::function<double(double)> function;
	ModifyerFunction(std::function<double(double)> function)
	: function(function) {};
	double operator()(double t) {
		return function(t);
	}
};

class Waveform {
public:
	virtual double operator()(double t, double freq) = 0;
};

class WaveformFunction : public Waveform {
public:
	std::function<double(double, double)> function;
	WaveformFunction(std::function<double(double, double)> function)
	: function(function) {};
	double operator()(double t, double freq) {
		return function(t, freq);
	}
};

class PianoWaveform : public Waveform {
public:
	inline PianoWaveform(void) {};
	inline double operator()(double t, double freq) {
		return 0.5 * Waves::piano(t, freq);
	};
};

class LinearAttack : public Modifyer {
	double a;
public:
	inline LinearAttack(double a) : a(a) {};
	inline double operator()(double t) {
		double val = (1.0 / a) * t;
		val *= (val >= 0.0);
		return (val > 1.0) ? 1.0 : val;
	};
};

class LinearRelease : public Modifyer {
	double a;
public:
	inline LinearRelease(double a) : a(a) {};
	inline double operator()(double t) {
		double val = (-1.0 / a) * (t - a);
		return (val < 0.0) ? 0.0 : val;
	};
};

class Generator {
public:
	Waveform* waveform = 0;
	Modifyer* attack = 0;
	Modifyer* release = 0;

	inline double sample(double t, double freq) {
		double value = (*waveform)(t, freq);
		if (attack) value *= (*attack)(t);
		if (release) value *= (*release)(t);
		return value;
	};
};

#endif