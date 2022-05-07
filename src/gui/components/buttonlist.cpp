#include <mkbd/gui/components/buttonlist.hpp>

ButtonListComponent::ButtonListComponent(Layout layout, int gap, std::vector<ButtonDetails> buttons, int fontSize, Color fgColor, Color bgColor)
: Component(layout), mGap(gap), mFontSize(fontSize), mButtonDetails(buttons), mFgColor(fgColor), mBgColor(bgColor) {
}

void ButtonListComponent::init(void) {
	int x = mLayout.x;
	int y = mLayout.y;
	int width = mLayout.width;
	int height = mLayout.height;
	for (const auto& b : mButtonDetails) {
		Layout layout(x, y, width, height, mLayout.details);
		ButtonComponent* bg = new ButtonComponent(layout, b.text, b.callback, mBgColor, mFgColor);
		bg->setFontSize(mFontSize);
		mWindow->addComponent(bg);
		y += height + mGap;
	}
}
