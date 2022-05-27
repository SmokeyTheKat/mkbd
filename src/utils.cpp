#include <mkbd/utils.hpp>

#include <cstdio>

namespace Utils {
	std::string runCommand(std::string_view cmd) {
		std::string result;

		FILE* fp = popen(cmd.data(), "r");
		char c;
		while ((c = fgetc(fp)) != EOF) {
			result.push_back(c);
		}
		pclose(fp);

		if (result.back() == '\n') {
			result.pop_back();
		}

		return result;
	}

	bool stringIsNumber(std::string str) {
		if (str.length() == 0)
			return false;
		
		for (auto& c : str) {
			if (!std::isdigit(c))
				return false;
		}
		return true;
	}
}
