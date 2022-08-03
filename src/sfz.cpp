#include <mkbd/sfz.hpp>

#include <mkbd/utils.hpp>

#include <iostream>

SfzParser::SfzParser(std::string path)
: mFile(path) {
	mFile.open();
};

bool SfzParser::isAtParamater(int at) {
	return mFile.matchNextByte('=', at) < mFile.matchNextWhiteSpace(at);
}

bool SfzParser::isAtHeader(int at) {
	return mFile[at] == '<';
}

bool SfzParser::isAtComment(int at) {
	return mFile[at] == '/';
}

void SfzParser::parseHeader(void) {
	mFile.pop(1);

	std::string header = mFile.popStringUntil('>');

	if (header == "control") {
		current = &control;
	} else if (header == "global") {
		current = &global;
		global.merge(control);
	} else if (header == "group") {
		groups.push_back(SfzGroup());
		current = &groups.back();
		currentGroup = &groups.back();
		current->merge(global);
		current->merge(control);
	} else if (header == "region") {
		if (!currentGroup) {
			groups.push_back(SfzGroup());
			currentGroup = &groups.back();
			currentGroup->merge(global);
		}
		currentGroup->regions.push_back(SfzRegion());
		current = &currentGroup->regions.back();
		current->merge(*currentGroup);
	}
}

int SfzParser::getParamaterEnd(void) {
	int i = 0;
	while (
		i < mFile.length() &&
		!isAtParamater(i) &&
		mFile[i-1] != '\n'
	) i++;
	return i - 1;
}

void SfzParser::parseParamater(void) {
	std::string name = mFile.popStringUntil('=');
	std::string value = mFile.popString(getParamaterEnd());
	current->set<std::string>(name, value);

}

void SfzParser::nextLine(void) {
	while (mFile.length() > 0 && mFile.front() != '\n') mFile.pop(1);
	mFile.pop(1);
}

std::vector<SfzRegion> SfzParser::parse(void) {
	while (mFile.length() > 0) {
		mFile.skipWhiteSpace();
		if (isAtComment()) {
			nextLine();
		} else if (isAtHeader()) {
			parseHeader();
		} else if (isAtParamater()) {
			parseParamater();
		} else {
			if (mFile.length() > 0) {
				mFile.pop(1);
			}
		}
	}

	std::cout << "\n";

	std::vector<SfzRegion> regions;
	for (auto& g : groups) {
		for (auto& r : g.regions) {
			regions.push_back(r);
		}
	}

	return regions;
}

