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

	Component* mParent = 0;
	std::vector<Component*> mChildren;

public:
	Component(Layout layout);

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

	SDL_Renderer* getRenderer(void);
	Rect getRect(void) { return Rect(mX, mY, mWidth, mHeight); };

	const std::vector<Component*>& getChildren(void) { return mChildren; };
	void addChild(Component* component);

	bool hasParent(void) { return mParent != 0; };
	Component* getParent(void) { return mParent; };
	void setParent(Component* component) { 
		mParent = component;
		if (!isChildOf(component)) {
			component->addChild(this);
		}
	};

	bool isChildOf(Component* component) {
		for (auto c : component->getChildren()) {
			if (c == this)
				return true;
		}
		return false;
	};

	void applyToChildren(std::function<void(Component*)> func) {
		for (Component* c : getChildren()) {
			func(c);
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

protected:
	void drawPoint(int x, int y);
	void drawLine(int x1, int y1, int x2, int y2);
	void fillRectangle(int x, int y, int w, int h);
	void drawRectangle(int x, int y, int w, int h);
	void drawRectangleWithOutline(int x, int y, int w, int h);
};

#endif