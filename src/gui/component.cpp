#include <mkbd/gui/component.hpp>

#include <mkbd/gui/window.hpp>

Component::Component(Layout layout)
: mLayout(layout) {};

void Component::draw(void) {};
void Component::init(void) {};
void Component::onResize(int width, int height) {};
void Component::onClick(int button, int x, int y) {};
void Component::onOffClick(int button, int x, int y) {};
void Component::onMouseUp(int button, int x, int y) {};
void Component::onHover(int x, int y) {};
void Component::onLeave(int x, int y) {};
void Component::onDrag(int x, int y) {};
void Component::onKeyDown(int key) {};
void Component::onKeyUp(int key) {};

SDL_Renderer* Component::getRenderer(void) {
	return mWindow->getRenderer();
}

void Component::setColor1(int r, int g, int b) {
	mColor1 = Color(r, g, b);
}

void Component::setColor1(void) {
	SDL_SetRenderDrawColor(getRenderer(), RGB_ARGS(mColor1), 255);
}

void Component::setColor2(int r, int g, int b) {
	mColor2 = Color(r, g, b);
}

void Component::setColor2(void) {
	SDL_SetRenderDrawColor(getRenderer(), RGB_ARGS(mColor2), 255);
}

void Component::setColor(int r, int g, int b) {
	setColor1(r, g, b);
	setColor2(r, g, b);
}

void Component::drawLine(int x1, int y1, int x2, int y2) {
	setColor1();
	SDL_RenderDrawLine(getRenderer(), mX + x1, mY + y1, mX + x2, mY + y2);
}

void Component::fillRectangle(int x, int y, int w, int h) {
	setColor1();
	SDL_Rect rect = {
		.x = x + mX,
		.y = y + mY,
		.w = w,
		.h = h,
	};
	SDL_RenderFillRect(getRenderer(), &rect);
}

void Component::drawRectangle(int x, int y, int w, int h) {
	setColor2();
	SDL_Rect rect = {
		.x = x + mX,
		.y = y + mY,
		.w = w,
		.h = h,
	};
	SDL_RenderDrawRect(getRenderer(), &rect);
}

void Component::drawRectangleWithOutline(int x, int y, int w, int h) {
	fillRectangle(x, y, w, h);
	drawRectangle(x, y, w, h);
}