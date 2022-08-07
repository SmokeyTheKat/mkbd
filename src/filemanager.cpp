#include <mkbd/filemanager.hpp>

#include <mkbd/utils.hpp>

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <filesystem>

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

	std::vector<std::string> getAllFolders(std::string path) {
		std::vector<std::string> folders;
		for (const auto& entry : std::filesystem::directory_iterator(path)) {
			folders.insert(folders.begin(), entry.path());
		}
		return folders;
	}

	std::string getPathBaseName(std::string path) {
		return path.substr(path.find_last_of("/\\") + 1);
	}

	std::string getPathDirectory(std::string path) {
		return path.substr(0, path.find_last_of("/\\"));
	}

};