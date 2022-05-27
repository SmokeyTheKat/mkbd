#ifndef __MKBD_GUI_DRAWAREA_HPP__
#define __MKBD_GUI_DRAWAREA_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/color.hpp>

#include <array>

class DrawAreaComponent : public Component {
	std::vector<Color> mData;
public:
	DrawAreaComponent(Layout layout);

	void draw(void);
	void resize(int width, int height);
	void init(void);

	std::vector<Color>& getData(void) { return mData; };
};

#endif