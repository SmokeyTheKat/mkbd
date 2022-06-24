#include <mkbd/gui/component.hpp>

#include <mkbd/gui/window.hpp>

#include <SDL2/SDL2_gfxPrimitives.h>

Component::Component(Layout layout)
: mLayout(layout) {};

void Component::draw(void) {};
void Component::init(void) {};
void Component::onResize(int width, int height) {};
void Component::onClick(int button, int x, int y) {};
void Component::onOffClick(int button, int x, int y) {};
void Component::onMouseUp(int button, int x, int y) {};
void Component::onHover(int x, int y) {};
void Component::onLeave(int x, int y) {};
void Component::onDrag(int x, int y) {};
void Component::onKeyDown(int key) {};
void Component::onKeyUp(int key) {};

void Component::sync(void) {
	applyToAllChildren([this](Component* c) {
		getWindow()->setComponentsSize(c);
	});
}

void Component::addChild(Component* component) {
	if (component == this)
		return;

	if (component->isChildOf(this))
		return;

	component->setParent(0);
	mChildren.push_back(component);
	component->setParent(this);

	if (!mWindow)
		return;

	component->setWindow(mWindow);
	mWindow->setComponentsSize(component);
	component->init();
}

void Component::removeChild(Component* child) {
	if (!hasChild(child)) return;

	auto it = std::find(mChildren.begin(), mChildren.end(), child);
	mChildren.erase(it);

	child->setParent(0);
}

void Component::setParent(Component* component) { 
	detachParent();
	mParent = component;

	if (component == 0) {
		mLayer = 1;
		return;
	}

	mLayer = component->getLayer() + 1;

	if (!isChildOf(component)) {
		component->addChild(this);
	}
}

bool Component::applyToSelfAndActiveChildren(std::function<bool(Component*)> func) {
	if (isActive()) {
		if (!func(this)) {
			return false;
		}
	}

	for (Component* c : getChildren()) {
		c->applyToSelfAndChildren(func);
	}
	return true;
}

bool Component::applyToSelfAndActiveChildrenReverse(std::function<bool(Component*)> func) {
	if (isActive()) {
		if (!func(this)) {
			return false;
		}
	}

	std::vector<Component*> children = getChildren();

	for (auto it = children.rbegin(); it != children.rend(); it++) {
		Component* c = *it;
		c->applyToSelfAndChildrenReverse(func);
	}
	return true;
}

bool Component::applyToSelfAndChildren(std::function<bool(Component*)> func) {
	if (!func(this)) {
		return false;
	}

	for (Component* c : getChildren()) {
		c->applyToSelfAndChildren(func);
	}
	return true;
}

bool Component::applyToSelfAndChildrenReverse(std::function<bool(Component*)> func) {
	if (!func(this)) {
		return false;
	}

	std::vector<Component*> children = getChildren();

	for (auto it = children.rbegin(); it != children.rend(); it++) {
		Component* c = *it;
		c->applyToSelfAndChildrenReverse(func);
	}
	return true;
}

SDL_Renderer* Component::getRenderer(void) {
	return mWindow->getRenderer();
}

void Component::setColor1(int r, int g, int b) {
	mColor1 = Color(r, g, b);
}

void Component::setColor1(void) {
	SDL_SetRenderDrawColor(getRenderer(), RGB_ARGS(mColor1), 255);
}

void Component::setColor2(int r, int g, int b) {
	mColor2 = Color(r, g, b);
}

void Component::setColor2(void) {
	SDL_SetRenderDrawColor(getRenderer(), RGB_ARGS(mColor2), 255);
}

void Component::setColor(int r, int g, int b) {
	setColor1(r, g, b);
	setColor2(r, g, b);
}

void Component::drawLine(int x1, int y1, int x2, int y2) {
	lineRGBA(getRenderer(), mX + x1, mY + y1, mX + x2, mY + y2, RGB_ARGS(mColor1), 255);
}

void Component::drawPoint(int x, int y) {
	setColor1();
	SDL_RenderDrawPoint(getRenderer(), mX + x, mY + y);
}

void Component::fillRoundedRectangle(int x, int y, int w, int h, int rad) {
	x += mX;
	y += mY;
	roundedBoxRGBA(getRenderer(), x, y, x+w, y+h, rad, RGB_ARGS(mColor1), 255);
//    if (2 * rad > h) return;
//    if (2 * rad > w) return;
	aacircleRGBA(getRenderer(), x+rad, y+rad, rad, RGB_ARGS(mColor1), 255);
	aacircleRGBA(getRenderer(), x+w-rad, y+rad, rad, RGB_ARGS(mColor1), 255);
	aacircleRGBA(getRenderer(), x+rad, y+h-rad, rad, RGB_ARGS(mColor1), 255);
	aacircleRGBA(getRenderer(), x+w-rad, y+h-rad, rad, RGB_ARGS(mColor1), 255);
}

void Component::drawRoundedRectangle(int x, int y, int w, int h, int rad) {
	x += mX;
	y += mY;
	roundedRectangleRGBA(getRenderer(), x, y, x+w+1, y+h+1, rad, RGB_ARGS(mColor2), 255);
}

void Component::fillRectangle(int x, int y, int w, int h) {
	x += mX;
	y += mY;
	boxRGBA(getRenderer(), x, y, x+w, y+h, RGB_ARGS(mColor1), 255);
}

void Component::drawRectangle(int x, int y, int w, int h) {
	x += mX;
	y += mY;
	rectangleRGBA(getRenderer(), x, y, x+w+1, y+h+1, RGB_ARGS(mColor2), 255);
}

void Component::drawRectangleWithOutline(int x, int y, int w, int h) {
	fillRectangle(x, y, w, h);
	drawRectangle(x, y, w, h);
}

void Component::drawCircle(int x0, int y0, int r, int t) {
	x0 += mX;
	y0 += mY;
	for (int i = 0; i < t; i++)
		circleRGBA(getRenderer(), x0, y0, r - i, RGB_ARGS(mColor2), 255);
}

void Component::fillCircle(int x0, int y0, int r) {
	x0 += mX;
	y0 += mY;
	filledCircleRGBA(getRenderer(), x0, y0, r, RGB_ARGS(mColor1), 255);
}

void Component::drawEllipse(int x0, int y0, int w, int h, int t) {
	x0 += mX;
	y0 += mY;

	std::vector<SDL_Point> data;
	int tt = t * t;
	int ww = w * w;
	int hh = h * h;
	int wt = ww - 2*t*w + tt;
	int ht = hh - 2*t*h + tt;
	for (int y = -h; y <= h; y++) {
		for (int x = -w; x <= w; x++) {
			int xx = x * x;
			int yy = y * y;
			if (yy * ww <= hh * ww - (hh * xx) && yy * wt > ht * wt - (ht * xx)) {
				data.push_back({x0 + x, y0 + y});
			}
		}
	}
	setColor1();
	SDL_RenderDrawPoints(getRenderer(), data.data(), data.size());
}

void Component::fillEllipse(int x0, int y0, int w, int h) {
	x0 += mX;
	y0 += mY;

	std::vector<SDL_Point> data;
	int ww = w * w;
	int hh = h * h;
	for (int y = -h; y <= h; y++) {
		for (int x = -w; x <= w; x++) {
			int xx = x * x;
			int yy = y * y;
			if (yy * ww <= hh * ww - (hh * xx)) {
				data.push_back({x0 + x, y0 + y});
			}
		}
	}
	setColor1();
	SDL_RenderDrawPoints(getRenderer(), data.data(), data.size());
}
