#ifndef __MKBD_SAMPLEMANAGER_HPP__
#define __MKBD_SAMPLEMANAGER_HPP__

#include <mkbd/sample.hpp>

#include <string>

namespace SampleManager {
	RawSample* loadSample(std::string path);
	void unloadSample(std::string path);
	void unloadSample(RawSample* sample);
};

#endif