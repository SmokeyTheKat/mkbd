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
	FC_AlignEnum mAlign = FC_ALIGN_LEFT;
	std::string mText;

public:
	TextComponent(Layout layout, std::string text, const char* fontPath, int fontSize, Color color);
	void draw(void);
	void init(void);

	void setText(std::string text) { mText = text; };
	std::string getText(void) { return mText; };
	void setAlign(FC_AlignEnum align) { mAlign = align; };
	void setColor(Color color) { mColor = color; };
	void setFontSize(int fontSize) { mFontSize = fontSize; };
	
private:
};

#endif