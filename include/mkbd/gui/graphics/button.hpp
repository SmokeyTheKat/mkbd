#ifndef __MKBD_GUI_BUTTON_HPP__
#define __MKBD_GUI_BUTTON_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/texture.hpp>
#include <mkbd/gui/graphics/button.hpp>
#include <mkbd/gui/graphics/text.hpp>
#include <mkbd/gui/layout.hpp>

#include <mkbd/utils.hpp>

#include <functional>
#include <string>

class ButtonGraphic : public Graphic {
	std::function<void(void)> mCallback = 0;
	TextGraphic mText;
	Color mFgColor = Color(0, 0, 0);
	Color mBgColor = Color(150, 150, 150);

public:
	ButtonGraphic(Layout layout, std::string text, std::function<void(void)> callback);
	ButtonGraphic(Layout layout, std::string text, std::function<void(void)> callback, Color bgColor, Color fgColor);

	void draw(void);
	void init(void);
	void onResize(int width, int height);
	void onClick(int button, int x, int y);
	void onlick(int button, int x, int y);

	inline void setFontSize(int fontSize) { mText.setFontSize(fontSize); };
};

#endif