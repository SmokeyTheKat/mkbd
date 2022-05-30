#include <mkbd/gui/components/checkbox.hpp>


CheckBoxComponent::CheckBoxComponent(Layout layout, Color onColor, Color offColor, CheckBoxType type)
: Component(layout), mOnColor(onColor), mOffColor(offColor), mType(type) {
}

void CheckBoxComponent::init(void) {
}

void CheckBoxComponent::onClick(int button, int x, int y) {
	mState = !mState;
	emit("Change", mState);
}

void CheckBoxComponent::draw(void) {
	setColor(RGB_ARGS(mOffColor));
	int r = 4;
	fillRoundedRectangle(0, 0, mWidth, mHeight, r);
	int wb = 2;
	setColor(RGB_ARGS(Colors::White));
	fillRoundedRectangle(wb, wb, mWidth - wb * 2, mHeight - wb * 2, r);
	int b = 8;
	if (mState == true) {
		setColor(RGB_ARGS(mOnColor));
		fillRoundedRectangle(b, b, mWidth - b * 2, mHeight - b * 2, r);
	}
}
