#ifndef __MKBD_APP_HPP__
#define __MKBD_APP_HPP__

#include <mkbd/gui/window.hpp>
#include <mkbd/utils.hpp>

#include <string>
#include <vector>

class App {
	std::vector<std::string> mArgs;
	Window mWindow;
	Color mFgColor = Color(186, 238, 255);
	Color mBgColor = Color(176, 205, 219);
	Color mAccColor = Color(8, 119, 188);
	int mMidiPort = 1;
	int mHeaderHeight = 60;

public:
	App(int argc, char** argv);
	int main(void);

private:
	inline int getWidth(void) { return mWindow.getWidth(); };
	inline int getHeight(void) { return mWindow.getHeight(); };

	void generateHeader(const char* title);
	void generateFooter(void);

	void mainMenuPage(void);
	void generateMainMenuHeader(void);
	void generateMainMenuButtons(void);

	void freePlayPage(void);

	void chooseKeyboardPage(void);
};

#endif