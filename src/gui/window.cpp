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

	SDL_SetWindowResizable(mWindow, SDL_TRUE);

	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	FontManager::setRenderer(mRenderer);

	mPages.push(ComponentPage());

	clearScreen();
}

Window::~Window(void) {
	close();
}

void Window::setComponentsSize(Component* g) {
	Layout layout = g->getLayout();
	if (layout.details & Layout::FillX) {
		g->setWidth(mWidth - layout.width);
	} else if (layout.details & Layout::PercentWidth) {
		g->setWidth(mWidth * (layout.width / 100.0));
	} else {
		g->setWidth(layout.width);
	}

	if (layout.details & Layout::FillY) {
		g->setHeight(mHeight - layout.height);
	} else if (layout.details & Layout::PercentHeight) {
		g->setHeight(mHeight * (layout.height / 100.0));
	} else {
		g->setHeight(layout.height);
	}

	if (layout.details & Layout::AnchorCenter) {
		g->setX(layout.x + (mWidth / 2));
		g->setY(layout.y + (mHeight / 2));
	} else if (layout.details & Layout::AnchorTopCenter) {
		g->setX(layout.x + (mWidth / 2));
		g->setY(layout.y);
	} else if (layout.details & Layout::AnchorBottomLeft) {
		g->setX(layout.x);
		g->setY(mHeight - layout.y);
	} else if (layout.details & Layout::AnchorTopRight) {
		g->setX(mWidth - layout.x);
		g->setY(layout.y);
	} else if (layout.details & Layout::AnchorBottomRight) {
		g->setX(mWidth - layout.x);
		g->setY(mHeight - layout.y);
	} else {
		g->setX(layout.x);
		g->setY(layout.y);
	}
}

void Window::close(void) {
	emit("Exit");
	mQuit = true;
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Window::newPage(void) {
	mPages.push(ComponentPage());
}

void Window::popPage(void) {
	mNewPage = true;
}

void Window::clearPage(void) {
	for (Component* g : mPages.top()) {
		delete g;
	}
	getPage().clear();
}

void Window::addComponent(Component* graphic) {
	graphic->setWindow(this);
	setComponentsSize(graphic);
	graphic->init();
	getPage().push_back(graphic);
}

void Window::removeComponent(Component* graphic) {
	ComponentPage& page = mPages.top();
	for (auto it = page.begin(); it != page.end(); it++) {
		if (*it == graphic) {
			page.erase(it);
			return;
		}
	}
}

void Window::handleMouseButtonDownEvent(const SDL_MouseButtonEvent& e) {
	mMouseIsDown = true;
	ComponentPage& page = getPage();
	bool focusedFound = false;
	page.forEachActiveReverse([&](Component* g) {
		if (g->getRect().isPointIntersecting(e.x, e.y)) {
			if (!focusedFound) {
				if (focused) focused->setFocused(false);
				g->setFocused(true);
				focused = g;
				focusedFound = true;
			}
			g->onClick(e.button, e.x - g->getX(), e.y - g->getY());
			g->emit("Click", e.button, e.x - g->getX(), e.y - g->getY());
		} else {
			g->onOffClick(e.button, e.x - g->getX(), e.y - g->getY());
			g->emit("OffClick", e.button, e.x - g->getX(), e.y - g->getY());
		}
		return true;
	});
}

void Window::handleMouseButtonUpEvent(const SDL_MouseButtonEvent& e) {
	mMouseIsDown = false;
	getPage().forEachActive([&](Component* g) {
		if (g->getRect().isPointIntersecting(e.x, e.y)) {
			g->onMouseUp(e.button, e.x - g->getX(), e.y - g->getY());
		}
		return true;
	});
}

void Window::handleMouseMotionEvent(const SDL_MouseMotionEvent& e) {
	getPage().forEachActive([&](Component* g) {
		if (g->getRect().isPointIntersecting(e.x, e.y)) {
			g->setHovered(true);
			g->onHover(e.x - g->getX(), e.y - g->getY());
			g->emit("Hover", e.x - g->getX(), e.y - g->getY());
			if (mMouseIsDown) g->onDrag(e.x - g->getX(), e.y - g->getY());
		} else if (g->isHovered()) {
			g->setHovered(false);
			g->onLeave(e.x - g->getX(), e.y - g->getY());
			g->emit("Leave", e.x - g->getX(), e.y - g->getY());
		}
		return true;
	});
}

void Window::handleKeyDownEvent(const SDL_KeyboardEvent& e) {
	emit("KeyDown", e.keysym.sym);
	getPage().forEachActive([&](Component* g) {
		g->onKeyDown(e.keysym.sym);
		g->emit("KeyDown", e.keysym.sym);
		return true;
	});
}

void Window::handleKeyUpEvent(const SDL_KeyboardEvent& e) {
	emit("KeyUp", e.keysym.sym);
	getPage().forEachActive([&](Component* g) {
		g->onKeyUp(e.keysym.sym);
		g->emit("KeyUp", e.keysym.sym);
		return true;
	});
}


void Window::handleWindowResizeEvent(int width, int height) {
	mWidth = width;
	mHeight = height;
	getPage().forEachActive([&](Component* g) {
		setComponentsSize(g);
		g->onResize(width, height);
		g->emit("Resize", width, height);
		return true;
	});
}

void Window::handleWindowEvent(const SDL_WindowEvent& e) {
	switch (e.event) {
		case SDL_WINDOWEVENT_SHOWN: {
		} break;
		case SDL_WINDOWEVENT_HIDDEN: {
		} break;
		case SDL_WINDOWEVENT_EXPOSED: {
		} break;
		case SDL_WINDOWEVENT_MOVED: {
		} break;
		case SDL_WINDOWEVENT_RESIZED: {
			handleWindowResizeEvent(e.data1, e.data2);
		} break;
		case SDL_WINDOWEVENT_SIZE_CHANGED: {
		} break;
		case SDL_WINDOWEVENT_MINIMIZED: {
		} break;
		case SDL_WINDOWEVENT_MAXIMIZED: {
		} break;
		case SDL_WINDOWEVENT_RESTORED: {
		} break;
		case SDL_WINDOWEVENT_ENTER: {
		} break;
		case SDL_WINDOWEVENT_LEAVE: {
		} break;
		case SDL_WINDOWEVENT_FOCUS_GAINED: {
		} break;
		case SDL_WINDOWEVENT_FOCUS_LOST: {
		} break;
		case SDL_WINDOWEVENT_CLOSE: {
		} break;
		case SDL_WINDOWEVENT_TAKE_FOCUS: {
		} break;
		case SDL_WINDOWEVENT_HIT_TEST: {
		} break;
	}
}

void Window::handleEvent(const SDL_Event& e) {
	switch (e.type) {
		case SDL_QUIT: {
			close();
		} break;
		case SDL_MOUSEMOTION: {
			handleMouseMotionEvent(e.motion);
		} break;
		case SDL_MOUSEBUTTONDOWN: {
			handleMouseButtonDownEvent(e.button);
		} break;
		case SDL_MOUSEBUTTONUP: {
			handleMouseButtonUpEvent(e.button);
		} break;
		case SDL_WINDOWEVENT: {
			handleWindowEvent(e.window);
		} break;
		case SDL_KEYDOWN: {
			if (e.key.repeat == 0)
				handleKeyDownEvent(e.key);
		} break;
		case SDL_KEYUP: {
			handleKeyUpEvent(e.key);
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
		SDL_Delay(1);
	}
}

void Window::pageLoop(void) {
	while (!mQuit && !mNewPage) {
		update();
		SDL_Delay(1);
	}
	if (mNewPage) {
		clearPage();
		mPages.pop();
	}
	mNewPage = false;
}

void Window::clearScreen(void) {
	SDL_SetRenderDrawColor(mRenderer, RGB_ARGS(mBgColor), 255);
	SDL_RenderClear(mRenderer);
}

void Window::draw(void) {
	clearScreen();
	for (Component* g : getPage()) {
		if (g->isVisible() && g->isActive()) g->draw();
		if (mQuit) return;
	}

	SDL_RenderPresent(mRenderer);
}