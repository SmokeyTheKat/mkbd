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
	TextGraphic mText;
	InputType mInputType;

	intmax_t mMaxValue;
	intmax_t mMinValue;

	Color mFgColor = Color(0, 0, 0);
	Color mBgColor = Color(150, 150, 150);

public:
	InputGraphic(Layout layout, std::string text, InputType inputType = InputType::Text);

	void draw(void);
	void init(void);
	void onResize(int width, int height);
	void onKeyDown(int key);

	void setFontSize(int fontSize) { mText.setFontSize(fontSize); };
	void setMaxValue(intmax_t maxValue) { mMaxValue = maxValue; };
	void setMinValue(intmax_t minValue) { mMinValue = minValue; };

private:
	void setTextLayout(void);
	bool keyFitsRestrictions(int key);
	bool stringFitsRestrictions(const std::string& str);
	bool keyIsPrintable(int key);
};

#endif