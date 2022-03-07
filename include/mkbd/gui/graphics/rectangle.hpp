#ifndef __MKBD_GUI_RECTANGLE_HPP__
#define __MKBD_GUI_RECTANGLE_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/gui/layout.hpp>

#include <SDL2/SDL.h>

#include <string>

class RectangleGraphic : public Graphic {
	Color mColor;
	Color mOutline;

public:
	RectangleGraphic(Layout layout, Color color, Color outline = Color::none());
	void draw(void);
	
private:
};

#endif