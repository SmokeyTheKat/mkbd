#include <mkbd/gui/window.hpp>

#include <mkbd/gui/fontmanager.hpp>
#include <mkbd/gui/texture.hpp>

#include <SDL2/SDL_image.h>

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

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Could not initialize everyting lol! Error: " << SDL_GetError() << "\n";
	}

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	FontManager::setRenderer(mRenderer);

	mPages.push(GraphicPage());

	clearScreen();
}

Window::~Window(void) {
	close();
}

void Window::close(void) {
	onExit(this);
	mQuit = true;
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Window::newPage(void) {
	mPages.push(GraphicPage());
}

void Window::popPage(void) {
	mPages.pop();
}

void Window::clearPage(void) {
	getPage().clear();
}

void Window::addGraphic(Graphic* graphic) {
	graphic->setWindow(this);
	graphic->init();
	getPage().push_back(graphic);
}

void Window::handleMouseButtonDownEvent(const SDL_MouseButtonEvent& e) {
	for (Graphic* g : getPage()) {
		if (g->getRect().isPointIntersecting(e.x, e.y)) {
			g->onClick(e.button, e.x - g->getX(), e.y - g->getY());
		}
	}
}

void Window::handleEvent(const SDL_Event& e) {
	switch (e.type) {
		case SDL_QUIT: {
			close();
		} break;
		case SDL_MOUSEBUTTONDOWN: {
			handleMouseButtonDownEvent(e.button);
		} break;
		case SDL_KEYDOWN: {
			if (e.key.keysym.sym == SDLK_q) {
				 this->~Window();
			}
		} break;
	}
}

void Window::handleEvents(void) {
	SDL_Event e;
	while (!mQuit && SDL_PollEvent(&e) != 0) {
		handleEvent(e);
	}
}

void Window::update(void) {
	draw();
	handleEvents();
}

void Window::eventLoop(void) {
	while (!mQuit) {
		update();
	}
}

void Window::clearScreen(void) {
	SDL_SetRenderDrawColor(mRenderer, RGB_ARGS(mBgColor), 255);
	SDL_RenderClear(mRenderer);
}

void Window::draw(void) {
	clearScreen();
	for (Graphic* g : getPage()) {
		g->draw();
		if (mQuit) return;
	}

	SDL_RenderPresent(mRenderer);
}