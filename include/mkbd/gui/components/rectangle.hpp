#ifndef __MKBD_GUI_RECTANGLE_HPP__
#define __MKBD_GUI_RECTANGLE_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/layout.hpp>

#include <SDL2/SDL.h>

#include <string>

class RectangleComponent : public Component {
	Color mColor;
	Color mOutline;

public:
	RectangleComponent(Layout layout, Color color, Color outline = Color::none());
	void draw(void);
	
private:
};

#endif