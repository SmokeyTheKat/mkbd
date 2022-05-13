#ifndef __MKBD_GUI_INCREMENT_HPP__
#define __MKBD_GUI_INCREMENT_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/components/input.hpp>
#include <mkbd/gui/layout.hpp>

#include <mkbd/utils.hpp>

#include <functional>
#include <string>

class IncrementComponent : public Component {
	InputComponent mInput;
	Color mFgColor = Color(0, 0, 0);
	Color mBgColor = Color(150, 150, 150);

public:
	IncrementComponent(Layout layout, Color bgColor, Color fgColor);

	void draw(void);
	void init(void);
	void onResize(int width, int height);
	void onClick(int button, int x, int y);
};

#endif