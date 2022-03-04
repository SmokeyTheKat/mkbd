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
	TextGraphic(int x, int y, int width, int height, std::string text, const char* fontPath, int fontSize, Color color);
	void draw(void);
	void init(void);

	inline void setText(std::string text) { mText = text; };
	inline void setFontSize(int fontSize) { mFontSize = fontSize; };
	
private:
};

#endif