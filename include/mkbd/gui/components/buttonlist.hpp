#ifndef __MKBD_GUI_BUTTON_LIST_HPP__
#define __MKBD_GUI_BUTTON_LIST_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/texture.hpp>
#include <mkbd/gui/components/button.hpp>
#include <mkbd/gui/components/text.hpp>
#include <mkbd/gui/layout.hpp>

#include <mkbd/utils.hpp>

#include <functional>
#include <string>
#include <vector>

struct ButtonDetails {
	const char* text;
	std::function<void(void)> callback;
};

class ButtonListComponent : public Component {
	std::vector<ButtonDetails> mButtonDetails;
	Color mFgColor, mBgColor;
	int mGap;
	int mFontSize;

public:
	ButtonListComponent(Layout layout, int gap, std::vector<ButtonDetails> buttons, int fontSize, Color fgColor, Color bgColor);

	void init(void);
};

#endif