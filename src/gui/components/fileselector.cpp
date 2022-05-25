#include <mkbd/gui/components/fileselector.hpp>

#include <mkbd/filemanager.hpp>

FileSelectorComponent::FileSelectorComponent(Layout layout, Color bgColor, Color fgColor)
: Component(layout), mBgColor(bgColor), mFgColor(fgColor) {
	mInput = new InputComponent(
		Layout(0, 0, 80, 100, Layout::PercentWidth | Layout::PercentHeight),
		""
	);

	mButton = new ButtonComponent(
		Layout(80, 0, 20, 100, Layout::PercentWidth | Layout::PercentHeight | Layout::PercentX),
		"",
		[](){},
		bgColor, fgColor
	);

	std::cout << "OMG\n";
	mButton->on("Click", FUNC([this], (int button, int x, int y), {
		mPath = FileManager::selectFile("Background Image");
		std::cout << mPath << " !!!!!!!\n";
		mInput->setText(mPath);
	}));

	addChild(mInput);
	addChild(mButton);
}


void FileSelectorComponent::draw(void) {
	setColor(RGB_ARGS(mBgColor));
}