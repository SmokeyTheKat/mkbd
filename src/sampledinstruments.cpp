#include <mkbd/instruments.hpp>

#include <mkbd/music.hpp>
#include <mkbd/filemanager.hpp>
#include <mkbd/waves.hpp>
#include <mkbd/sfz.hpp>
#include <mkbd/samplemanager.hpp>

#include <limits>
#include <valarray>
#include <complex>
#include <string>

SampledInstrument::SampledInstrument(const std::string& name, const std::string& path, int low, int high, SampleFormat format)
: mName(name), mPath(path), mLowestNote(low), mHighestNote(high), mFormat(format) {
	mVolume = 1;
	std::cout << name << "\n";

	mSampleGroups.resize(mHighestNote);

	mGen.attack = LinearAttack<5>;
	mGen.release = Cutoff<3000>;
	mGen.fadeOut = LinearRelease<200>;
	mGen.waveform = std::bind(&SampledInstrument::waveform, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
};

double SampledInstrument::waveform(double t, double freq, double vel) {
	int note = Music::freqToNote(freq);

	if (note < mLowestNote || note >= mHighestNote)
		return 0;

	if (mSampleGroups[note].size() == 0) 
		return 0;

	Sample* sample = &mSampleGroups[note][0];
	for (auto& s : mSampleGroups[note]) {
		if (s.isVelocityInRange(vel)) {
			sample = &s;
			break;
		}
	}

	t *= sample->getShiftFreqRatio();
	t *= sample->getOffsetFreqRatio();

	double idx = t * 100.0;

	return sample->get(idx) * mVolume;
}

int SampledInstrument::getLowestKey(void) {
	for (int i = 0; i < mSampleGroups.size(); i++) {
		if (mSampleGroups[i].size() > 0) 
			return i;
	}
	return 21;
}

int SampledInstrument::getHighestKey(void) {
	for (int i = mSampleGroups.size()-1; i >= 0; i--) {
		if (mSampleGroups[i].size() > 0) 
			return i;
	}
	return 108;
}

void SampledInstrument::load(void) {
	unload();

	SfzParser p(mPath + "/sfz.sfz");
	std::vector<SfzRegion> regions = p.parse();
	for (auto& r : regions) {
		if (r.has("sample")) {
			int keyCenter;
			int lowKey;
			int highKey;

			if (r.has("key")) {
				int key = r.get<int>("key");
				keyCenter = key;
				lowKey = key;
				highKey = key;
			} 

			if (r.has("pitch_keycenter"))
				keyCenter = r.get<int>("pitch_keycenter");

			if (r.has("lokey"))
				lowKey = r.get<int>("lokey");

			if (r.has("hikey"))
				highKey = r.get<int>("hikey");

			std::string defaultPath = r.tryGet<std::string>("default_path", "");

			std::string path = r.get<std::string>("sample");
			RawSample* rawSample = SampleManager::loadSample(mPath + "/" + defaultPath + path);


			int offset = r.tryGet<int>("offset", 0);
			int end = r.tryGet<int>("end", rawSample->length);

			double lowVel = r.tryGet<double>("lovel", 0);
			double highVel = r.tryGet<double>("hivel", 127);


			for (int i = lowKey; i <= highKey; i++) {
				Sample sample(rawSample);
				sample.setRange(offset, end);
				sample.setVelocityRange(lowVel, highVel);
				sample.setDistanceFromKeyCenter(i - keyCenter);
				sample.setVolume(r.tryGet<double>("volume", 1));
				sample.setPitchOffset(r.tryGet<double>("pitch", 0));

				mSampleGroups[i].push_back(sample);
			}
		}
	}
}

void SampledInstrument::unload(void) {
	for (auto& sg : mSampleGroups) {
		for (auto& s : sg) {
			SampleManager::unloadSample(s.getRawSample());
		}
		sg.clear();
	}
}
