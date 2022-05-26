#ifndef __MKBD_WINDOW_HPP__
#define __MKBD_WINDOW_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/texture.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/utils.hpp>
#include <mkbd/eventemitter.hpp>

#include <SDL2/SDL.h>

#include <vector>
#include <functional>
#include <stack>

struct ComponentPage : public std::vector<Component*> {
	void forEachActive(std::function<bool(Component*)> func) {
		for (Component* c : *this) {
			if (!c->applyToSelfAndActiveChildren(func))
				break;
		}
	}

	void forEachActiveReverse(std::function<bool(Component*)> func) {
		for (auto it = rbegin(); it != rend(); it++) {
			Component* c = *it;
			if (!c->applyToSelfAndActiveChildrenReverse(func)) 
				break;
		}
	}

	void forEach(std::function<bool(Component*)> func) {
		for (Component* c : *this) {
			if (!c->applyToSelfAndChildren(func))
				break;
		}
	}

	void forEachReverse(std::function<bool(Component*)> func) {
		for (auto it = rbegin(); it != rend(); it++) {
			Component* c = *it;
			if (!c->applyToSelfAndChildrenReverse(func)) 
				break;
		}
	}
};

class Window : public EventEmitter {
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer = 0;
	Color mBgColor = Color(0, 0, 0);

	Component* focused = 0;
	
	std::stack<ComponentPage> mPages;

	bool mMouseIsDown = false;

	int mHeight;
	int mWidth;
	bool mQuit = false;
	bool mNewPage = false;

	SDL_Cursor* mCursor = 0;

public:
	Window(int width, int height);
	~Window(void);

	void close(void);

	void syncComponent(Component* component);
	void addComponent(Component* graphic);
	void removeComponent(Component* graphic);
	void clearPage(void);
	void popPage(void);
	void newPage(void);
	ComponentPage& getPage(void) { return mPages.top(); };

	void setComponentsSize(Component* g);

	SDL_Renderer* getRenderer(void) { return mRenderer; };
	void update(void);
	void handleEvents(void);
	void handleEvent(const SDL_Event& e);
	void eventLoop(void);
	void pageLoop(void);

	int getWidth(void) { return mWidth; };
	int getHeight(void) { return mHeight; };

	void setCursor(SDL_SystemCursor cursor);

	void setBgColor(int r, int g, int b) { mBgColor = Color(r, g, b); };

private:
	void clearScreen(void);
	void draw(void);

	void handleMouseButtonDownEvent(const SDL_MouseButtonEvent& e);
	void handleMouseButtonUpEvent(const SDL_MouseButtonEvent& e);
	void handleMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void handleWindowEvent(const SDL_WindowEvent& e);
	void handleKeyDownEvent(const SDL_KeyboardEvent& e);
	void handleKeyUpEvent(const SDL_KeyboardEvent& e);
	void handleWindowResizeEvent(int width, int height);
};

#endif