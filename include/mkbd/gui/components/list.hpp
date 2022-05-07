#ifndef __MKBD_GUI_LIST_HPP__
#define __MKBD_GUI_LIST_HPP__

#include <mkbd/gui/layout.hpp>

#include <functional>

using ComponentCreater = std::function<Component*(Layout layout)>;

namespace ListDirection {
	enum {
		Vertical,
		Horizontal,
	};
}

template<class T = Component>
inline std::vector<T*> createListComponent(Layout layout, std::vector<ComponentCreater> creaters, int gap, int direction = ListDirection::Vertical) {
	std::vector<T*> graphics;

	int x = layout.x;
	int y = layout.y;
	int width = layout.width;
	int height = layout.height;

	for (auto creater : creaters) {
		Layout newLayout(x, y, width, height, layout.details);
		graphics.push_back((T*)creater(newLayout));

		if (direction == ListDirection::Vertical)
			y += height + gap;
		else if (direction == ListDirection::Horizontal)
			x += width + gap;
	}

	return graphics;
}

#endif