#ifndef __MKBD_GUI_TEXT_HPP__
#define __MKBD_GUI_TEXT_HPP__

#include <mkbd/gui/graphic.hpp>

#include <SDL2/SDL.h>

#include <string>

class TextGraphic : public Graphic {
	const char* mFontPath;
	Color mColor;
	int mFontSize;
	std::string mText;

public:
	TextGraphic(std::string text, const char* fontPath, int fontSize, Color color, int x, int y, int width, int height);
	void draw(void);
	inline void setText(std::string text) { mText = text; };
	
private:
};

#endif