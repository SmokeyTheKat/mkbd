#ifndef __MKBD_GUI_TEXT_HPP__
#define __MKBD_GUI_TEXT_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/gui/fontmanager.hpp>
#include <mkbd/gui/layout.hpp>

#include <SDL2/SDL.h>

#include <string>

class TextGraphic : public Graphic {
	const char* mFontPath;
	Color mColor;
	int mFontSize;
	FC_AlignEnum mAlign = FC_ALIGN_LEFT;
	std::string mText;

public:
	TextGraphic(Layout layout, std::string text, const char* fontPath, int fontSize, Color color);
	void draw(void);
	void init(void);

	inline void setText(std::string text) { mText = text; };
	inline std::string getText(void) { return mText; };
	inline void setAlign(FC_AlignEnum align) { mAlign = align; };
	inline void setFontSize(int fontSize) { mFontSize = fontSize; };
	
private:
};

#endif