#ifndef __MKBD_FILEMANAGER_HPP__
#define __MKBD_FILEMANAGER_HPP__

#include <string>
#include <vector>

namespace FileManager {
	std::string selectFile(std::string title, std::string startPath = "");
	bool fileExists(std::string path);
	std::vector<std::string> getAllFolders(std::string path = ".");
	std::string getPathBaseName(std::string path);
	std::string getPathDirectory(std::string path);
};


#endif