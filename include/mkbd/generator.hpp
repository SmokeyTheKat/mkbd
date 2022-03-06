#ifndef __MKBD_GENERATOR_HPP__
#define __MKBD_GENERATOR_HPP__

#include <mkbd/waves.hpp>

#include <functional>

typedef std::function<double(double,double)> Waveform;
typedef std::function<double(double)> Modifyer;

template<int A>
double LinearAttack(double t) {
	double value = (1.0 / A) * t;
	value *= (value >= 0);
	return (value > 1.0) ? 1.0 : value;
}

template<int A>
double LinearRelease(double t) {
	double value = (-1.0 / A) * (t - A);
	value *= (value >= 0);
	return (value < 0.0) ? 0.0 : value;
}

template<int A>
double Constant(double t) {
	return A;
}

struct Generator {
	Waveform waveform = 0;
	Modifyer attack = Constant<1>;
	Modifyer release = Constant<1>;
	Modifyer fadeOut = Constant<0>;

	inline double getModifyers(double t) {
		double value = 1;
		value *= attack(t);
		value *= release(t);
		return value;
	}

	inline double sample(double t, double freq) {
		return waveform(t, freq) * getModifyers(t);
	};
};

#endif