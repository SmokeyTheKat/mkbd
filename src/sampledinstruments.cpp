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
};

double SampledInstrument::waveform(double t, double freq, double vel) {
	int note = Music::freqToNote(freq);

	if (note < mLowestNote || note >= mHighestNote)
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

void SampledInstrument::load(void) {
	mGen.attack = LinearAttack<5>;
	mGen.release = Cutoff<3000>;
	mGen.fadeOut = LinearRelease<200>;
	mGen.waveform = std::bind(&SampledInstrument::waveform, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

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
//    for (int i = mLowestNote; i < mSamples.size() && i < mHighestNote; i++) {
//        NoteSample& ns = mSamples[i];
//
//        if (ns.path.length() == 0) {
//            if (mFormat == SampleFormat::Names)
//                ns.path = mSamplePath + "ff." + Music::getNoteFullName(i) + ".wav";
//            else if (mFormat == SampleFormat::Numbers)
//                ns.path = mSamplePath + std::to_string(i) + ".wav";
//        }
//        if (FileManager::fileExists(ns.path.c_str())) {
//            SDL_LoadWAV(ns.path.c_str(), &ns.spec, (uint8_t**)&ns.buffer, &ns.length);
//            ns.length /= sizeof(int16_t);
//        }
//    }
//    for (int i = mLowestNote; i < mSamples.size() && i < mHighestNote; i++) {
//        NoteSample& ns = mSamples[i];
//        if (ns.length == 0) {
//            ns.shift = getClosestNoteTo(i) - i;
//        }
//    }
}

int SampledInstrument::getClosestNoteTo(int to) {
//    int clUp = 0;
//    for (int i = to + 1; i < mSamples.size() && i < mHighestNote; i++) {
//        NoteSample& ns = mSamples[i];
//        if (ns.length > 0) {
//            clUp = i;
//            break;
//        }
//    }
//
//    int clDown = 0;
//    for (int i = to - 1; i >= 0; i--) {
//        NoteSample& ns = mSamples[i];
//        if (ns.length > 0) {
//            clDown = i;
//            break;
//        }
//    }
//
//    if (to - clDown < clUp - to || clUp == 0) {
//        return clDown;
//    } else {
//        return clUp;
//    }

	return 0;
}

void SampledInstrument::unload(void) {
//    for (int i = mLowestNote; i < mSamples.size() && i < mHighestNote; i++) {
//        const NoteSample& ns = mSamples[i];
//        SDL_FreeWAV((uint8_t*)ns.buffer);
//    }
}
