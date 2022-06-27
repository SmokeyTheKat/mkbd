#include <mkbd/gui/components/frame.hpp>
#include <mkbd/gui/components/rectangle.hpp>

FrameComponent::FrameComponent(Layout layout, int dragBarHeight)
: Component(layout), mDragBarHeight(dragBarHeight) {
	Layout areaLayout = mLayout.resetPosition();
	areaLayout.y = mDragBarHeight;
	areaLayout.height -= mDragBarHeight;
//    mArea = new RectangleComponent(areaLayout, Colors::Yellow);
	mArea = new Component(areaLayout);
	Component::addChild(mArea);
}

void FrameComponent::addChild(Component* component) {
	mArea->addChild(component);
}

void FrameComponent::init(void) {
}

void FrameComponent::onClick(int button, int x, int y) {
	if (y > mDragBarHeight) return;

	mIX = x;
	mIY = y;
}

void FrameComponent::onDrag(int x, int y) {
	if (y > mDragBarHeight) return;

	int nx = x - mIX;
	int ny = y - mIY;
	Layout l = getLayout();
	l.x += nx;
	l.y += ny;
	setLayout(l);
}

void FrameComponent::draw(void) {
	setColor(RGB_ARGS(Colors::Black));
	fillRectangle(0, 0, mWidth, mDragBarHeight);
}
