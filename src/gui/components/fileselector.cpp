#include <mkbd/gui/components/fileselector.hpp>

#include <mkbd/filemanager.hpp>
#include <SDL2/SDL.h>

FileSelectorComponent::FileSelectorComponent(Layout layout, Color bgColor, Color fgColor)
: Component(layout), mBgColor(bgColor), mFgColor(fgColor) {
	mInput = new InputComponent(
		Layout(0, 0, 80, 100, Layout::PercentWidth | Layout::PercentHeight),
		""
	);

	mButton = new ButtonComponent(
		Layout(80, 0, 20, 100, Layout::PercentWidth | Layout::PercentHeight | Layout::PercentX),
		"File",
		[](){},
		bgColor, Colors::White
	);
	mButton->setFontSize(15);

	mButton->on("Click", FUNC([this], (int button, int x, int y), {
//        mPath = FileManager::selectFile("Background Image");
		mInput->setText(mPath);
		mWindow->setCursor(SDL_SYSTEM_CURSOR_CROSSHAIR);
	}));

	addChild(mButton);
	addChild(mInput);
}


void FileSelectorComponent::draw(void) {
	setColor(RGB_ARGS(mBgColor));
}