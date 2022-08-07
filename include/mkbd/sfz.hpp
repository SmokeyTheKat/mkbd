#ifndef __MKBD_SFZ_HPP__
#define __MKBD_SFZ_HPP__

#include <mkbd/utils.hpp>

#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <unordered_map>

using SfzDefine = std::pair<std::string, std::string>;

struct SfzParamaters {
	std::unordered_map<std::string, std::string> paramaters;

	template<class T>
	T get(std::string name) {
		std::stringstream ss(paramaters[name]);
		T value;
		ss >> value;
		return value;
	};

	template<class T>
	T tryGet(std::string name, T defaultValue) {
		if (has(name)) {
			return get<T>(name);
		} else {
			return defaultValue;
		}
	};

	template<class T>
	void set(std::string name, T value) {
		paramaters[name] = std::to_string(value);
	};

	bool has(std::string name) {
		return paramaters.count(name) > 0;
	};

	void merge(const SfzParamaters& other) {
		paramaters.insert(other.paramaters.begin(), other.paramaters.end());
	};

	void clear(void) {
		paramaters.clear();
	};
};

template<>
inline std::string SfzParamaters::get<std::string>(std::string name) {
	return paramaters[name];
}

template<>
inline void SfzParamaters::set<std::string>(std::string name, std::string value) {
	paramaters[name] = value;
}

struct SfzControl : SfzParamaters {
};

struct SfzGlobal : SfzParamaters {
};

struct SfzRegion : SfzParamaters {
};

struct SfzGroup : SfzParamaters {
	std::vector<SfzRegion> regions;
};

struct SfzFile {
	std::string filePath;

	SfzControl control;
	SfzGlobal global;

	std::vector<SfzGroup> groups;

	SfzFile(void) {};
	SfzFile(std::string path) { load(path); };
	void load(std::string path);
};

class SfzParser {
	File mFile;

	SfzControl control;
	SfzGlobal global;

	SfzParamaters* current = &global;
	SfzGroup* currentGroup = 0;

	std::vector<SfzGroup> groups;

	std::vector<SfzDefine> mDefines;
	
public:
	SfzParser(std::string path);
	std::vector<SfzRegion> parse(void);

private:
	void parseHeader(void);
	void parseParamater(void);

	int getParamaterEnd(void);

	void parseMacro(void);

	void includeFile(void);

	void registerDefine(void);
	std::vector<SfzDefine>::iterator getDefineByName(std::string name);
	void applyDefines(std::string& data);
	std::string applyDefines(const std::string&& data);

	bool isAtParamater(int at = 0);
	bool isAtMacro(int at = 0);
	bool isAtHeader(int at = 0);
	bool isAtComment(int at = 0);

	void nextLine(void);

};

#endif