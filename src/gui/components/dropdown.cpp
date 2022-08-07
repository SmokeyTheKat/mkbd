#include <mkbd/gui/components/dropdown.hpp>

void DropDownComponent::init(void) {
}

void DropDownComponent::onClick(int b, int x, int y) {
	if (y < mCollaspedHeight) {
		mIsCollasped = !mIsCollasped;
	
		emit("DropDown");
	} else if (!mIsCollasped) {
	}
}

void DropDownComponent::draw(void) {
	setColor(255, 255, 0);
	fillRectangle(0, 0, mWidth, mCollaspedHeight);
	if (!mIsCollasped) {
		setColor(0, 0, 255);
		fillRectangle(0, mCollaspedHeight, mWidth, mHeight);
	}
}
