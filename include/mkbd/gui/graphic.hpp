#ifndef __MKBD_GRAPHIC_HPP__
#define __MKBD_GRAPHIC_HPP__

#include <mkbd/utils.hpp>

#include <SDL2/SDL.h>

#include <vector>

class Graphic {
public:
	SDL_Renderer* mRenderer;
	int mX;
	int mY;
	int mWidth;
	int mHeight;
	Color mColor1;
	Color mColor2;

	Graphic(int x, int y, int width, int height);

	virtual void draw(void) = 0;
	void setRenderer(SDL_Renderer* renderer);
	void setColor(int r, int g, int b);
	void setColor1(int r, int g, int b);
	void setColor1(void);
	void setColor2(int r, int g, int b);
	void setColor2(void);
	void drawLine(int x1, int y1, int x2, int y2);
	void fillRectangle(int x, int y, int w, int h);
	void drawRectangle(int x, int y, int w, int h);
	void drawRectangleWithOutline(int x, int y, int w, int h);
};

#endif