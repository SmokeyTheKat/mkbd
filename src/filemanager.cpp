#include <mkbd/filemanager.hpp>

#include <mkbd/utils.hpp>

#include <string>
#include <sstream>

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
};