#include <mkbd/sfz.hpp>

#include <mkbd/utils.hpp>
#include <mkbd/filemanager.hpp>

#include <iostream>

SfzParser::SfzParser(std::string path)
: mFile(path) {
	mFile.open();
};

bool SfzParser::isAtParamater(int at) {
	return mFile.matchNextByte('=', at) < mFile.matchNextWhiteSpace(at);
}

bool SfzParser::isAtMacro(int at) {
	std::string macro = mFile.getWordAt(at);
	return macro == "#define" ||
		   macro == "#include";
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
		!isAtMacro(i) &&
		mFile[i-1] != '\n'
	) i++;
	return i - 1;
}

void SfzParser::parseParamater(void) {
	std::string name = applyDefines(mFile.popStringUntil('='));
	std::string value = applyDefines(mFile.popString(getParamaterEnd()));
	current->set<std::string>(name, value);

}

void SfzParser::parseMacro(void) {
	mFile.pop(1);
	std::string macro = mFile.popWord();

	if (macro == "define") {
		registerDefine();
	}
	else if (macro == "include") {
		includeFile();
	}
}

void SfzParser::includeFile(void) {
	mFile.pop(1);
	std::string path = applyDefines(mFile.popStringUntil('"'));
	mFile.insert(File::readAll(FileManager::getPathDirectory(mFile.getPath()) + "/" + path));
}

void SfzParser::registerDefine(void) {
	std::string name = mFile.popWord();
	std::string value = mFile.popWord();

	auto it = getDefineByName(name);
	if (it == mDefines.end()) {
		mDefines.push_back(make_pair(name, value));
	} else {
		it->second = value;
	}
}

std::vector<SfzDefine>::iterator SfzParser::getDefineByName(std::string name) {
	for (auto it = mDefines.begin(); it != mDefines.end(); it++) {
		if (it->first == name) {
			return it;
		}
	}
	return mDefines.end();
}

std::string SfzParser::applyDefines(const std::string&& data) {
	std::string out = data;
	applyDefines(out);
	return out;
}

void SfzParser::applyDefines(std::string& data) {
	for (auto def : mDefines) {
		const std::string& name = def.first;
		const std::string& value = def.second;

		int pos = data.find(name);
		while (pos != std::string::npos) {
			data.replace(pos, name.length(), value);
			pos = data.find(name);
		}
	}
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
		} else if (isAtMacro()) {
			parseMacro();
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
