#include <mkbd/gui/components/drawarea.hpp>
DrawAreaComponent::DrawAreaComponent(Layout layout)
: Component(layout) {
}

void DrawAreaComponent::init(void) {
	mData.resize(mWidth * mHeight);
}

void DrawAreaComponent::resize(int width, int height) {
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