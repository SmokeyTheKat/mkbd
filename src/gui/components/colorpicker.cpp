#include <mkbd/gui/components/colorpicker.hpp>

#include <mkbd/settings.hpp>

ColorPickerComponent::ColorPickerComponent(Layout layout, Color* colorOut)
: Component(layout), mColorOut(colorOut) {
	mDrawArea = new DrawAreaComponent(layout);
	addChild(mDrawArea);
};

void ColorPickerComponent::init(void) {
	std::vector<Color>& data = mDrawArea->getData();
	int i = 0;
	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			double v = 100.0 * ((double)x / (double)mWidth);
			double h = 360.0 * ((double)y / (double)mHeight);
			double s = 100.0;
			Color c(h, s, v, ColorFormat::Hsv);
			c.toRgb();
			setColor1(RGB_ARGS(c));
			data[i++] = c;
		}
	}
}

void ColorPickerComponent::onClick(int button, int x, int y) {
	onDrag(x, y);
}

void ColorPickerComponent::onDrag(int x, int y) {
	*mColorOut = mDrawArea->getData()[y * mWidth + x];
	emit("Change");
}

void ColorPickerComponent::draw(void) {
	
}

