#ifndef __MKBD_COMPONENT_DROPDOWN_HPP__
#define __MKBD_COMPONENT_DROPDOWN_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/eventemitter.hpp>

#include <string>

class DropDownComponent : public Component {
	std::vector<std::string> mData;
	int mCollaspedHeight = 20;
	bool mIsCollasped = true;

public:
	DropDownComponent(Layout layout)
	: Component(layout) {};

	void draw(void);
	void init(void);
	void onClick(int b, int x, int y);

	bool isCollasped(void) { return mIsCollasped; };

	void setData(std::vector<std::string> data) { mData = data; };
};

#endif