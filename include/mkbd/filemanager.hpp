#ifndef __MKBD_FILEMANAGER_HPP__
#define __MKBD_FILEMANAGER_HPP__

#include <string>

namespace FileManager {
	std::string selectFile(std::string title, std::string startPath = "");
};


#endif