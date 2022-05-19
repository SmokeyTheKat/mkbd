#include <mkbd/filemanager.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>

namespace FileManager {
	std::string selectFile(std::string title, std::string startPath) {
		std::stringstream ssCmd;
		ssCmd << "zenity --file-selection "
			  << "--title='" << title << "\' "
			  << "--filename='`realpath '" << startPath << "'`' ";

		std::string cmd = ssCmd.str();

		std::string resultPath;

		FILE* fp = popen(cmd.c_str(), "r");
		char c;
		while ((c = fgetc(fp)) != EOF) {
			resultPath.push_back(c);
		}
		pclose(fp);

		return resultPath;
	}
};