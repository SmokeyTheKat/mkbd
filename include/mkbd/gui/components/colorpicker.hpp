#ifndef __MKBD_GUI_COLORPICKER_HPP__
#define __MKBD_GUI_COLORPICKER_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/gui/texture.hpp>
#include <mkbd/gui/components/button.hpp>
#include <mkbd/gui/components/text.hpp>
#include <mkbd/gui/components/drawarea.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/utils.hpp>

#include <functional>
#include <string>

class ColorPickerComponent : public Component {
	DrawAreaComponent* mDrawArea;
	Color* mColorOut;

public:
	ColorPickerComponent(Layout layout, Color* colorOut);

	void init(void);
	void draw(void);
	void onClick(int button, int x, int y);
	void onDrag(int x, int y);
};

#endif