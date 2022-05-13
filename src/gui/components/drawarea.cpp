#include <mkbd/gui/components/drawarea.hpp>
DrawAreaComponent::DrawAreaComponent(Layout layout)
: Component(layout) {
	mData.resize(mWidth * mHeight);
}

void DrawAreaComponent::init(void) {
}

void DrawAreaComponent::draw(void) {
	for (int y = 0; y < mHeight; y++) {
		for (int x = 0; x < mWidth; x++) {
			Color& c = mData[y * mWidth + x];
			setColor1(RGB_ARGS(c));
			drawPoint(x, y);
		}
	}
}