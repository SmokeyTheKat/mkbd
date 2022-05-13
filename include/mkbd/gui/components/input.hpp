#ifndef __MKBD_GUI_INPUT_HPP__
#define __MKBD_GUI_INPUT_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/components/text.hpp>
#include <mkbd/gui/window.hpp>
#include <mkbd/eventemitter.hpp>

#include <mkbd/utils.hpp>

#include <functional>
#include <string>

enum class InputType {
	Text,
	Number,
};

class InputComponent : public Component {
	TextComponent* mText;
	InputType mInputType;

	int mCursor = 0;

	intmax_t mMaxValue = 1000000000000;
	intmax_t mMinValue = -1000000000000;

	Color mFgColor = Color(0, 0, 0);
	Color mBgColor = Color(150, 150, 150);

public:
	InputComponent(Layout layout, std::string text, InputType inputType = InputType::Text);

	void draw(void);
	void init(void);
	void onResize(int width, int height);
	void onKeyDown(int key);

	void setText(std::string text);
	void setAlign(FC_AlignEnum align);
	void setFontSize(int fontSize) { mText->setFontSize(fontSize); };
	void setMaxValue(intmax_t maxValue) { mMaxValue = maxValue; };
	void setMinValue(intmax_t minValue) { mMinValue = minValue; };

	void setBgColor(Color color) { mBgColor = color; };
	void setFgColor(Color color) {
		mFgColor = color;
		mText->setColor(color);
	};

	TextComponent& getTextComponent(void) { return *mText; };

private:
	void setTextLayout(void);
	bool keyFitsRestrictions(int key);
	bool stringFitsRestrictions(const std::string& str);
	bool keyIsPrintable(int key);
};

#endif