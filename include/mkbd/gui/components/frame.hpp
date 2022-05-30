#ifndef __MKBD_GUI_FRAME_HPP__
#define __MKBD_GUI_FRAME_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/utils.hpp>

#include <functional>
#include <string>

class FrameComponent : public Component {
	int mDragBarHeight;
	Component* mArea;

	int mIX;
	int mIY;

public:
	FrameComponent(Layout layout, int dragBarHeight = 15);

	void addChild(Component* component);

	void init(void);
	void onClick(int button, int x, int y);
	void onDrag(int x, int y);
	void draw(void);
};

#endif