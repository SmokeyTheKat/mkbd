#include <mkbd/gui/graphic.hpp>

#include <mkbd/gui/window.hpp>

Graphic::Graphic(Layout layout)
: mLayout(layout) {};

void Graphic::draw(void) {};
void Graphic::init(void) {};
void Graphic::onResize(int width, int height) {};
void Graphic::onClick(int button, int x, int y) {};
void Graphic::onMouseUp(int button, int x, int y) {};
void Graphic::onHover(int x, int y) {};
void Graphic::onLeave(int x, int y) {};
void Graphic::onDrag(int x, int y) {};
void Graphic::onKeyDown(int key) {};

SDL_Renderer* Graphic::getRenderer(void) {
	return mWindow->getRenderer();
}

void Graphic::setColor1(int r, int g, int b) {
	mColor1 = Color(r, g, b);
}

void Graphic::setColor1(void) {
	SDL_SetRenderDrawColor(getRenderer(), RGB_ARGS(mColor1), 255);
}

void Graphic::setColor2(int r, int g, int b) {
	mColor2 = Color(r, g, b);
}

void Graphic::setColor2(void) {
	SDL_SetRenderDrawColor(getRenderer(), RGB_ARGS(mColor2), 255);
}

void Graphic::setColor(int r, int g, int b) {
	setColor1(r, g, b);
	setColor2(r, g, b);
}

void Graphic::drawLine(int x1, int y1, int x2, int y2) {
	setColor1();
	SDL_RenderDrawLine(getRenderer(), mX + x1, mY + y1, mX + x2, mY + y2);
}

void Graphic::fillRectangle(int x, int y, int w, int h) {
	setColor1();
	SDL_Rect rect = {
		.x = x + mX,
		.y = y + mY,
		.w = w,
		.h = h,
	};
	SDL_RenderFillRect(getRenderer(), &rect);
}

void Graphic::drawRectangle(int x, int y, int w, int h) {
	setColor2();
	SDL_Rect rect = {
		.x = x + mX,
		.y = y + mY,
		.w = w,
		.h = h,
	};
	SDL_RenderDrawRect(getRenderer(), &rect);
}

void Graphic::drawRectangleWithOutline(int x, int y, int w, int h) {
	fillRectangle(x, y, w, h);
	drawRectangle(x, y, w, h);
}