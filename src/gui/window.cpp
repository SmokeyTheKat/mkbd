#include <mkbd/gui/window.hpp>

#include <mkbd/gui/fontmanager.hpp>

#include <iostream>

Window::Window(int width, int height)
: mWidth(width), mHeight(height) {
	mWindow = SDL_CreateWindow(
		"Window", SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		mWidth,
		mHeight,
		SDL_WINDOW_SHOWN
	);
	mRenderer =  SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	FontManager::setRenderer(mRenderer);
	clearScreen();
}

Window::~Window(void) {
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Window::addGraphic(Graphic* graphic) {
	graphic->setRenderer(mRenderer);
	mGraphics.push_back(graphic);
}

void Window::update(void) {
	draw();
	SDL_Event e;
	while (SDL_PollEvent(&e) != 0) {
		if (e.type == SDL_QUIT) {
			mQuit = true;
		}
		else if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.sym) {
				case SDLK_s: {
				} break;
				default: {
				} break;
			}
		}
	}
}

void Window::handleEvents(void) {
	while (mQuit) {
		update();
	}
}

void Window::clearScreen(void) {
	SDL_RenderClear(mRenderer);
}

void Window::draw(void) {
	clearScreen();
	for (Graphic* g : mGraphics) {
		g->draw();
	}


	SDL_RenderPresent(mRenderer);
}