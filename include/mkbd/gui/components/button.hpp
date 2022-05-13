#ifndef __MKBD_GUI_BUTTON_HPP__
#define __MKBD_GUI_BUTTON_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/texture.hpp>
#include <mkbd/gui/components/button.hpp>
#include <mkbd/gui/components/text.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/eventemitter.hpp>

#include <mkbd/utils.hpp>

#include <functional>
#include <string>

class ButtonComponent : public Component {
	std::function<void(void)> mCallback = 0;
	TextComponent* mText;
	Color mFgColor = Color(0, 0, 0);
	Color mBgColor = Color(150, 150, 150);

public:
	ButtonComponent(Layout layout, std::string text, std::function<void(void)> callback, Color bgColor, Color fgColor);
	ButtonComponent(Layout layout, std::string text, std::function<void(void)> callback)
	: ButtonComponent(layout, text, callback, Color(0), Color(150)) {};

	void draw(void);
	void init(void);
	void onResize(int width, int height);
	void onClick(int button, int x, int y);
	void onlick(int button, int x, int y);

	void setFontSize(int fontSize) { mText->setFontSize(fontSize); };
	void setBgColor(Color color) { mBgColor = color; };
	TextComponent& getText(void) { return *mText; };
};

#endif