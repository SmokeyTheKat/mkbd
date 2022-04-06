#ifndef __MKBD_GUI_INPUT_HPP__
#define __MKBD_GUI_INPUT_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/gui/graphics/text.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/eventemitter.hpp>

#include <mkbd/utils.hpp>

#include <functional>
#include <string>

enum class InputType {
	Text,
	Number,
};

class InputGraphic : public Graphic, public EventEmitter {
	std::function<void(void)> mCallback = 0;
	TextGraphic mText;
	InputType mInputType;

	Color mFgColor = Color(0, 0, 0);
	Color mBgColor = Color(150, 150, 150);

public:
	InputGraphic(Layout layout, std::string text, std::function<void(void)> callback, InputType inputType = InputType::Text);

	void draw(void);
	void init(void);
	void onResize(int width, int height);
	void onKeyDown(int key);

	inline void setFontSize(int fontSize) { mText.setFontSize(fontSize); };

private:
	void setTextLayout(void);
};

#endif