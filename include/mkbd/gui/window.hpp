#ifndef __MKBD_WINDOW_HPP__
#define __MKBD_WINDOW_HPP__

#include <mkbd/gui/graphic.hpp>
#include <mkbd/utils.hpp>

#include <SDL2/SDL.h>

#include <vector>

class Window {
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	std::vector<Graphic*> mGraphics;
	int mHeight;
	int mWidth;
	bool mQuit = false;

public:
	Window(int width, int height);
	~Window(void);
	void addGraphic(Graphic* graphic);
	void update(void);
	void handleEvents(void);

private:
	void clearScreen(void);
	void draw(void);
};

#endif