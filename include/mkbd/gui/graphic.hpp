#ifndef __MKBD_GRAPHIC_HPP__
#define __MKBD_GRAPHIC_HPP__

#include <mkbd/utils.hpp>
#include <mkbd/rect.hpp>

#include <SDL2/SDL.h>

#include <vector>

class Graphic {
protected:
	class Window* mWindow;
	int mX;
	int mY;
	int mWidth;
	int mHeight;
	Color mColor1;
	Color mColor2;

public:
	Graphic(int x, int y, int width, int height);

	virtual void draw(void);
	virtual void init(void);
	virtual void onClick(int button, int x, int y);

	inline void setWindow(Window* window) { mWindow = window; };
	void setColor(int r, int g, int b);
	void setColor1(int r, int g, int b);
	void setColor1(void);
	void setColor2(int r, int g, int b);
	void setColor2(void);

	SDL_Renderer* getRenderer(void);
	inline Rect getRect(void) { return Rect(mX, mY, mWidth, mHeight); };
	inline int getX(void) { return mX; };
	inline int getY(void) { return mY; };

	void drawLine(int x1, int y1, int x2, int y2);
	void fillRectangle(int x, int y, int w, int h);
	void drawRectangle(int x, int y, int w, int h);
	void drawRectangleWithOutline(int x, int y, int w, int h);
};

#endif