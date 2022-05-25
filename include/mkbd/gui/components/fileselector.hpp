#ifndef __MKBD_GUI_FILESELECTOR_HPP__
#define __MKBD_GUI_FILESELECTOR_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/layout.hpp>

#include <mkbd/gui/components/input.hpp>
#include <mkbd/gui/components/button.hpp>

#include <mkbd/utils.hpp>

#include <string>

class FileSelectorComponent : public Component {
	Color mFgColor = Color(0, 0, 0);
	Color mBgColor = Color(150, 150, 150);
	std::string mPath;

	InputComponent* mInput;
	ButtonComponent* mButton;

public:
	FileSelectorComponent(Layout layout, Color bgColor, Color fgColor);

	void draw(void);

	std::string getPath(void) { return mPath; };
};

#endif