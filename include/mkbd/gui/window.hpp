#ifndef __MKBD_WINDOW_HPP__
#define __MKBD_WINDOW_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/gui/texture.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/utils.hpp>

#include <SDL2/SDL.h>

#include <vector>
#include <functional>
#include <stack>

using GraphicPage = std::vector<Graphic*>;

class Window {
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer = 0;
	Color mBgColor = Color(0, 0, 0);
	
	std::stack<GraphicPage> mPages;

	int mHeight;
	int mWidth;
	bool mQuit = false;
	bool mNewPage = false;

public:
	Window(int width, int height);
	~Window(void);

	void close(void);

	void addGraphic(Graphic* graphic);
	void clearPage(void);
	void popPage(void);
	void newPage(void);
	inline GraphicPage& getPage(void) { return mPages.top(); };

	void setGraphicsSize(Graphic* g);

	inline SDL_Renderer* getRenderer(void) { return mRenderer; };
	void update(void);
	void handleEvents(void);
	void handleEvent(const SDL_Event& e);
	void eventLoop(void);
	void pageLoop(void);

	inline int getWidth(void) { return mWidth; };
	inline int getHeight(void) { return mHeight; };

	inline void setBgColor(int r, int g, int b) { mBgColor = Color(r, g, b); };

	std::function<void(Window* window)> onExit = 0;

private:
	void clearScreen(void);
	void draw(void);

	void handleMouseButtonDownEvent(const SDL_MouseButtonEvent& e);
	void handleMouseMotionEvent(const SDL_MouseMotionEvent& e);
	void handleWindowEvent(const SDL_WindowEvent& e);
	void handleWindowResizeEvent(int width, int height);
};

#endif