#ifndef __MKBD_GUI_CONTAINER_GRAPHIC_HPP__
#define __MKBD_GUI_CONTAINER_GRAPHIC_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/layout.hpp>

#include <mkbd/utils.hpp>

#include <vector>

class ContainerComponent : public Component {
	std::vector<Component*> mChildren;

public:
	ContainerComponent(void);
	void draw(void);
	void init(void);
	void onResize(int width, int height);
	void onClick(int button, int x, int y);
	void onlick(int button, int x, int y);
};

#endif