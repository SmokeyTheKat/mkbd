#ifndef __MKBD_GRAPHIC_HPP__
#define __MKBD_GRAPHIC_HPP__

#include <mkbd/utils.hpp>
#include <mkbd/rect.hpp>
#include <mkbd/gui/layout.hpp>

#include <SDL2/SDL.h>

#include <vector>

class Window;

class Graphic {
protected:
	Window* mWindow;
	Layout mLayout;
	int mX;
	int mY;
	int mWidth;
	int mHeight;

	bool mIsHovered = true;
	bool mIsFocused = false;

	Color mColor1;
	Color mColor2;

public:
	Graphic(Layout layout);

	virtual void draw(void);
	virtual void init(void);
	virtual void onClick(int button, int x, int y);
	virtual void onMouseUp(int button, int x, int y);
	virtual void onResize(int width, int height);
	virtual void onHover(int x, int y);
	virtual void onDrag(int x, int y);
	virtual void onLeave(int x, int y);
	virtual void onKeyDown(int key);

	inline void setWindow(Window* window) { mWindow = window; };
	inline Window* getWindow(void) { return mWindow; };

	void setColor(int r, int g, int b);
	void setColor1(int r, int g, int b);
	void setColor1(void);
	void setColor2(int r, int g, int b);
	void setColor2(void);

	SDL_Renderer* getRenderer(void);
	inline Rect getRect(void) { return Rect(mX, mY, mWidth, mHeight); };

	inline int getX(void) { return mX; };
	inline void setX(int x) { mX = x; };

	inline int getY(void) { return mY; };
	inline void setY(int y) { mY = y; };

	inline int getWidth(void) { return mWidth; };
	inline void setWidth(int width) { mWidth = width; };
	inline int getHeight(void) { return mHeight; };
	inline void setHeight(int height) { mHeight = height; };

	inline Layout& getLayout(void) { return mLayout; };

	inline bool isHovered(void) { return mIsHovered; };
	inline void setHovered(bool hovered) { mIsHovered = hovered; };

	inline bool isFocused(void) { return mIsFocused; };
	inline void setFocused(bool focused) { mIsFocused = focused; };

	void drawLine(int x1, int y1, int x2, int y2);
	void fillRectangle(int x, int y, int w, int h);
	void drawRectangle(int x, int y, int w, int h);
	void drawRectangleWithOutline(int x, int y, int w, int h);
};

#endif