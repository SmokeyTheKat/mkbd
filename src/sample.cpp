#include <mkbd/sample.hpp>

#include <mkbd/math.hpp>
#include <mkbd/music.hpp>

Sample::Sample(RawSample* rawSample)
: mRawSample(rawSample), mData(rawSample->buffer, 0, rawSample->length) {
	
}

double Sample::get(double point) {
	if (point > length()) 
		return 0;

	double bitRatio = std::pow(2, getBitRate()) / std::pow(2, 16);

	int leftIdx = getChannelCount() * std::floor(point);
	int rightIdx = getChannelCount() * std::ceil(point);

	double leftSample = 0;
	double rightSample = 0;

	for (int i = 0; i < getChannelCount(); i++) {
		leftSample += mData.getNumberAt(getBitRate(), leftIdx + i);
		rightSample += mData.getNumberAt(getBitRate(), rightIdx + i);
	}

	return
		lerp(leftSample, rightSample, point - leftIdx / getChannelCount()) *
		mVolume /
		20000.0 /
		bitRatio;
}

double Sample::getShiftFreqRatio(void) {
	return Music::centsToFreqRatio(mDistanceFromKeyCenter * 100);
}

double Sample::getOffsetFreqRatio(void) {
	return Music::centsToFreqRatio(mPitchOffset * 100);
}
