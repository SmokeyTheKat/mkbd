#ifndef __MKBD_GUI_TEXT_HPP__
#define __MKBD_GUI_TEXT_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/fontmanager.hpp>
#include <mkbd/gui/layout.hpp>

#include <SDL2/SDL.h>

#include <string>

class TextComponent : public Component {
	const char* mFontPath;
	Color mColor;
	int mFontSize;
	VAlign mVAlign = VAlign::Top;
	HAlign mHAlign = HAlign::Left;
	std::string mText;

	struct dtx_font* mFont = 0;

	unsigned int mTexture;

public:
	TextComponent(Layout layout, std::string text, const char* fontPath, int fontSize, Color color);
	~TextComponent(void) {};

	void draw(void);
	void onResize(int width, int height);
	void init(void);

	void setText(std::string text) { mText = text; };
	std::string getText(void) { return mText; };
	void setVAlign(VAlign align) { mVAlign = align; };
	void setHAlign(HAlign align) { mHAlign = align; };
	void setColor(Color color) { mColor = color; };
	void setFontSize(int fontSize) { mFontSize = fontSize; generateFont(); };
	
private:
	void generateFont(void);
};

#endif