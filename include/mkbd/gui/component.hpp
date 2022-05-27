#ifndef __MKBD_GRAPHIC_HPP__
#define __MKBD_GRAPHIC_HPP__

#include <mkbd/utils.hpp>
#include <mkbd/rect.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/eventemitter.hpp>

#include <SDL2/SDL.h>

#include <vector>

class Window;

class Component : public EventEmitter {
protected:
	Window* mWindow = 0;
	Layout mLayout;
	int mX;
	int mY;
	int mWidth;
	int mHeight;

	bool mIsHovered = true;
	bool mIsFocused = false;
	bool mIsVisible = true;
	bool mIsActive = true;

	Color mColor1;
	Color mColor2;

	SDL_SystemCursor mCursor = SDL_NUM_SYSTEM_CURSORS;

	Component* mParent = 0;
	std::vector<Component*> mChildren;

	bool mAutoDraw = true;

	int mLayer = 1;

public:
	Component(Layout layout);
	virtual ~Component(void) { freeAllChildren(); };

	virtual void draw(void);
	virtual void init(void);
	virtual void onClick(int button, int x, int y);
	virtual void onOffClick(int button, int x, int y);
	virtual void onMouseUp(int button, int x, int y);
	virtual void onResize(int width, int height);
	virtual void onHover(int x, int y);
	virtual void onDrag(int x, int y);
	virtual void onLeave(int x, int y);
	virtual void onKeyDown(int key);
	virtual void onKeyUp(int key);

	void setWindow(Window* window) { mWindow = window; };
	Window* getWindow(void) { return mWindow; };

	void setColor(int r, int g, int b);
	void setColor1(int r, int g, int b);
	void setColor1(void);
	void setColor2(int r, int g, int b);
	void setColor2(void);

	void setCursor(SDL_SystemCursor cursor) { mCursor = cursor; };
	SDL_SystemCursor getCursor(void) { return mCursor; };

	SDL_Renderer* getRenderer(void);
	Rect getRect(void) { return Rect(mX, mY, mWidth, mHeight); };

	void leftOf(Component* c, int border = 0) {
		Layout l = c->getLayout();
		mLayout.details = l.details;
		mLayout.y = l.y;
		mLayout.x = l.x - l.width / 2 - mLayout.width / 2 - border;
	};

	int getLayer(void) { return mLayer; };
	void setLayer(int layer) { mLayer = layer; };

	const std::vector<Component*>& getChildren(void) { return mChildren; };
	void addChild(Component* component);
	void removeChild(Component* child);
	void detachParent(void) { 
		if (mParent) mParent->removeChild(this);
	};

	void freeAllChildren(void) {
		for (auto c : mChildren) {
			delete c;
		}
	};

	bool hasParent(void) { return mParent != 0; };
	Component* getParent(void) { return mParent; };
	void setParent(Component* component);

	bool hasChild(Component* child) {
		return std::find(mChildren.begin(), mChildren.end(), child) != mChildren.end();
	};

	bool isChildOf(Component* component) { return component->hasChild(this); };

	void applyToChildren(std::function<void(Component*)> func) {
		for (Component* c : getChildren()) {
			func(c);
		}
	}
	void applyToAllChildren(std::function<void(Component*)> func) {
		for (Component* c : getChildren()) {
			func(c);
			c->applyToAllChildren(func);
		}
	}

	bool applyToSelfAndActiveChildren(std::function<bool(Component*)> func);
	bool applyToSelfAndActiveChildrenReverse(std::function<bool(Component*)> func);
	bool applyToSelfAndChildren(std::function<bool(Component*)> func);
	bool applyToSelfAndChildrenReverse(std::function<bool(Component*)> func);

	int getX(void) { return mX; };
	void setX(int x) { mX = x; };

	int getY(void) { return mY; };
	void setY(int y) { mY = y; };

	int getWidth(void) { return mWidth; };
	void setWidth(int width) { mWidth = width; };
	int getHeight(void) { return mHeight; };
	void setHeight(int height) { mHeight = height; };

	bool isAutoDrawOn(void) { return mAutoDraw; };
	void setAutoDraw(bool state) { mAutoDraw = state; };

	Layout& getLayout(void) { return mLayout; };
	void setLayout(Layout layout) { mLayout = layout; };

	bool isHovered(void) { return mIsHovered; };
	void setHovered(bool hovered) { mIsHovered = hovered; };

	bool isVisible(void) { return mIsVisible; };
	void setSelfVisible(bool visible) { mIsVisible = visible; };
	void setVisible(bool visible) {
		setSelfVisible(visible);
		applyToChildren([visible](Component* c) {
			c->setVisible(visible);
		});
	};

	bool isFocused(void) { return mIsFocused; };
	void setFocused(bool focused) { mIsFocused = focused; };

	bool isActive(void) { return mIsActive; };
	void setSelfActive(bool active) { mIsActive = active; };
	void setActive(bool active) {
		setSelfActive(active);
		applyToChildren([active](Component* c) {
			c->setActive(active);
		});
	};

	void drawPoint(int x, int y);
	void drawLine(int x1, int y1, int x2, int y2);
	void fillRectangle(int x, int y, int w, int h);
	void drawRectangle(int x, int y, int w, int h);
	void drawRectangleWithOutline(int x, int y, int w, int h);
	void drawCircle(int x0, int y0, int r, int t = 1);
	void fillCircle(int x, int y, int r);
	void drawEllipse(int x0, int y0, int w, int h, int t);
	void fillEllipse(int x0, int y0, int w, int h);
};

#endif