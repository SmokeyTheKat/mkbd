#include <mkbd/gui/graphic.hpp>

Graphic::Graphic(int x, int y, int width, int height)
: mX(x), mY(y), mWidth(width), mHeight(height) {};

void Graphic::setRenderer(SDL_Renderer* renderer) {
	mRenderer = renderer;
}

void Graphic::setColor1(int r, int g, int b) {
	mColor1 = Color(r, g, b);
}

void Graphic::setColor1(void) {
	SDL_SetRenderDrawColor(mRenderer, RGB_ARGS(mColor1), 255);
}

void Graphic::setColor2(int r, int g, int b) {
	mColor2 = Color(r, g, b);
}

void Graphic::setColor2(void) {
	SDL_SetRenderDrawColor(mRenderer, RGB_ARGS(mColor2), 255);
}

void Graphic::setColor(int r, int g, int b) {
	setColor1(r, g, b);
	setColor2(r, g, b);
}

void drawLine(int x1, int y1, int x2, int y2) {
	SDL_RenderDrawLine(mRenderer, x1, y1, x2, y2);
}

void Graphic::fillRectangle(int x, int y, int w, int h) {
	setColor1();
	SDL_Rect rect = {
		.x = x + mX,
		.y = y + mY,
		.w = w,
		.h = h,
	};
	SDL_RenderFillRect(mRenderer, &rect);
}

void Graphic::drawRectangle(int x, int y, int w, int h) {
	setColor2();
	SDL_Rect rect = {
		.x = x + mX,
		.y = y + mY,
		.w = w,
		.h = h,
	};
	SDL_RenderDrawRect(mRenderer, &rect);
}

void Graphic::drawRectangleWithOutline(int x, int y, int w, int h) {
	fillRectangle(x, y, w, h);
	drawRectangle(x, y, w, h);
}