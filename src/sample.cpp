#include <mkbd/sample.hpp>

#include <mkbd/math.hpp>
#include <mkbd/music.hpp>

Sample::Sample(RawSample* rawSample)
: mRawSample(rawSample), mData(rawSample->buffer, 0, rawSample->length) {
	
}


float interpolateHermite4pt3oX(float x0, float x1, float x2, float x3, float t) {
	float c0 = x1;
	float c1 = 0.5 * (x2 - x0);
	float c2 = x0 - (2.5 * x1) + (2 * x2) - (0.5 * x3);
	float c3 = (0.5 * (x3 - x0)) + (1.5 * (x1 - x2));
	return (((((c3 * t) + c2) * t) + c1) * t) + c0;
}

float lerp (float a, float b, float weight) {
	float v = weight * weight * weight * (weight * (weight * 6.0f - 15.0f) + 10.0f);
	return a + v * (b - a);
}

double Sample::get(double point) {
	if (point > length()) 
		return 0;

	int leftIdx = 2 * std::floor(point);
	int rightIdx = 2 * std::ceil(point);

	double leftSample = mData[leftIdx] + mData[leftIdx+1];
	double rightSample = mData[rightIdx] + mData[rightIdx+1];

	return rmap(
		point,
		leftIdx / 2, rightIdx / 2,
		leftSample, rightSample
	) * mVolume / 20000.0;
}

double Sample::getShiftFreqRatio(void) {
	return Music::centsToFreqRatio(mDistanceFromKeyCenter * 100);
}

double Sample::getOffsetFreqRatio(void) {
	return Music::centsToFreqRatio(mPitchOffset * 100);
}
