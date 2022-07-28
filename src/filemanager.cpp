#include <mkbd/filemanager.hpp>

#include <mkbd/utils.hpp>

#include <string>
#include <sstream>
#include <fstream>

namespace FileManager {
	std::string selectFile(std::string title, std::string startPath) {
		std::stringstream ssCmd;
		ssCmd << "zenity --file-selection "
			  << "--title='" << title << "\' "
			  << "--filename='`realpath '" << startPath << "'`' ";

		std::string cmd = ssCmd.str();

		std::string resultPath = Utils::runCommand(cmd);

		return resultPath;
	}

	bool fileExists(std::string path) {
		std::ifstream fs(path);
		return fs.good();
	}

};