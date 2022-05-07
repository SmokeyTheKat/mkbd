#ifndef __MKBD_GENERATOR_HPP__
#define __MKBD_GENERATOR_HPP__

#include <mkbd/waves.hpp>
#include <mkbd/utils.hpp>

#include <functional>
#include <iostream>

typedef std::function<double(double,double)> Waveform;
typedef std::function<double(double)> Modifyer;

template<int A>
static double LinearAttack(double t);
template<int A>
static double LinearRelease(double t);
template<int A>
static double Cutoff(double t);
template<int A>
static double Constant(double t);

struct Generator {
	Waveform waveform = 0;
	Modifyer attack = Constant<1>;
	Modifyer release = Constant<1>;
	Modifyer fadeOut = Constant<0>;

	double getModifyers(double t) {
		return attack(t) * release(t);
	}

	double sample(double t, double freq) {
		return waveform(t, freq) * getModifyers(t);
	};
};

template<int A>
static double LinearAttack(double t) {
	if (A == 0) return 1;
	double value = (1.0 / (double)A) * t;
	value = MAX(value, 0);
	return MIN(value, 1);
}

template<int A>
static double LinearRelease(double t) {
	if (A == 0) return 0;
	double value = (-1.0 / (double)A) * (t - (double)A);
	value = MIN(value, 1);
	return MAX(value, 0);
}

template<int A>
static double Cutoff(double t) {
	if ((int)t >= A) {
		return 0.0;
	}
	return 1.0;
}

template<int A>
static double Constant(double t) {
	return A;
}

#endif