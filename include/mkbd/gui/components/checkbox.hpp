#ifndef __MKBD_GUI_CHECKBOX_HPP__
#define __MKBD_GUI_CHECKBOX_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/layout.hpp>

#include <mkbd/utils.hpp>

#include <functional>
#include <string>

enum class CheckBoxType {
	Box,
	Slider
};

class CheckBoxComponent : public Component {
	Color mOnColor;
	Color mOffColor;
	CheckBoxType mType;
	bool mState = false;

public:
	CheckBoxComponent(Layout layout, Color onColor, Color offColor, CheckBoxType type = CheckBoxType::Box);

	void init(void);

	void onClick(int button, int x, int y);

	void draw(void);
};

#endif