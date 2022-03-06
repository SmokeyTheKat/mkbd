#include <mkbd/gui/graphics/buttonlist.hpp>

ButtonListGraphic::ButtonListGraphic(Layout layout, int gap, std::vector<ButtonDetails> buttons, int fontSize, Color fgColor, Color bgColor)
: Graphic(layout), mGap(gap), mFontSize(fontSize), mButtonDetails(buttons), mFgColor(fgColor), mBgColor(bgColor) {
}

void ButtonListGraphic::init(void) {
	int x = mLayout.x;
	int y = mLayout.y;
	int width = mLayout.width;
	int height = mLayout.height;
	for (const auto& b : mButtonDetails) {
		Layout layout(x, y, width, height, mLayout.details);
		ButtonGraphic* bg = new ButtonGraphic(layout, b.text, b.callback, mBgColor, mFgColor);
		bg->setFontSize(mFontSize);
		mWindow->addGraphic(bg);
		y += height + mGap;
	}
}
