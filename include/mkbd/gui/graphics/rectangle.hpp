#ifndef __MKBD_GUI_RECTANGLE_HPP__
#define __MKBD_GUI_RECTANGLE_HPP__

#include <mkbd/gui/graphic.hpp>

#include <SDL2/SDL.h>

#include <string>

class RectangleGraphic : public Graphic {
	Color mColor;

public:
	RectangleGraphic(int x, int y, int width, int height, Color color);
	void draw(void);
	
private:
};

#endif