#ifndef __MKBD_MATH_HPP__
#define __MKBD_MATH_HPP__

double rmap(double v, double l1, double h1, double l2, double h2);
double roundTo(double value, double to);
bool isAbout(double value, double about, double threshold = 0.01);
double freqToRad(double freq);

#endif