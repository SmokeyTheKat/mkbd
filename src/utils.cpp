#include <mkbd/utils.hpp>

#include <fstream>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <cstdio>

void File::open(void) {
	std::ifstream fs(mPath, std::ios::binary);
	mData = std::vector<byte>(std::istreambuf_iterator<char>(fs), {});
};

namespace Utils {
	int getMillsFromTime(double time) {
		return (time - std::floor(time)) * 1000;
	}

	int getSecsFromTime(double time) {
		return (time - getMinsFromTime(time)) * 1000;
	}

	int getMinsFromTime(double time) {
		return time / 60.0;
	}

	std::string timeToString(double time) {
		int mins = time / 60.0;
		time -= mins * 60.0;

		int secs = time;
		time -= secs;

		int milsecs = time * 10.0;

		std::stringstream ss;
		ss << std::setfill('0');
		ss << std::setw(2) << mins
		   << ":"
		   << std::setw(2) << secs
		   << "."
		   << milsecs;

		return ss.str();
	}

	int getVarIntLength(const std::vector<byte>& data) {
		int count = 0;
		while (data[count++] >= 128);
		return count;
	}

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
