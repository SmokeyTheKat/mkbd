#ifndef __MKBD_GUI_LIST_HPP__
#define __MKBD_GUI_LIST_HPP__

#include <mkbd/gui/layout.hpp>

#include <functional>

using GraphicCreater = std::function<Graphic*(Layout layout)>;

namespace ListDirection {
	enum {
		Vertical,
		Horizontal,
	};
}

inline std::vector<Graphic*> createListGraphic(Layout layout, std::vector<GraphicCreater> creaters, int gap, int direction = ListDirection::Vertical) {
	std::vector<Graphic*> graphics;

	int x = layout.x;
	int y = layout.y;
	int width = layout.width;
	int height = layout.height;

	for (auto creater : creaters) {
		Layout newLayout(x, y, width, height, layout.details);
		graphics.push_back(creater(newLayout));

		if (direction == ListDirection::Vertical)
			y += height + gap;
		else if (direction == ListDirection::Horizontal)
			x += width + gap;
	}

	return graphics;
}

#endif