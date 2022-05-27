#include <mkbd/gui/component.hpp>

#include <mkbd/gui/window.hpp>

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

void Component::addChild(Component* component) {
	if (component == this)
		return;

	if (component->isChildOf(this))
		return;

	component->removeParent();

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
		if (!func(c)) {
			return false;
		}
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
		if (!func(c)) {
			return false;
		}
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
	setColor1();
	SDL_RenderDrawLine(getRenderer(), mX + x1, mY + y1, mX + x2, mY + y2);
}

void Component::drawPoint(int x, int y) {
	setColor1();
	SDL_RenderDrawPoint(getRenderer(), mX + x, mY + y);
}

void Component::fillRectangle(int x, int y, int w, int h) {
	setColor1();
	SDL_Rect rect = {
		.x = x + mX,
		.y = y + mY,
		.w = w,
		.h = h,
	};
	SDL_RenderFillRect(getRenderer(), &rect);
}

void Component::drawRectangle(int x, int y, int w, int h) {
	setColor2();
	SDL_Rect rect = {
		.x = x + mX,
		.y = y + mY,
		.w = w,
		.h = h,
	};
	SDL_RenderDrawRect(getRenderer(), &rect);
}

void Component::drawRectangleWithOutline(int x, int y, int w, int h) {
	fillRectangle(x, y, w, h);
	drawRectangle(x, y, w, h);
}

void Component::drawCircle(int x0, int y0, int r, int t) {
	x0 += mX;
	y0 += mY;

	std::vector<SDL_Point> data;
	int rr = r * r;
	int tt = t * t;
	int tr = rr - 2*t*r + tt;
	for (int y = -r; y <= r; y++) {
		for (int x = -r; x <= r; x++) {
			int xx = x * x;
			int yy = y * y;
			if (xx + yy <= rr && xx + yy > tr) {
				data.push_back({x0 + x, y0 + y});
			}
		}
	}
	setColor1();
	SDL_RenderDrawPoints(getRenderer(), data.data(), data.size());
}

void Component::fillCircle(int x0, int y0, int r) {
	x0 += mX;
	y0 += mY;

	std::vector<SDL_Point> data;
	int rr = r * r;
	for (int y = -r; y <= r; y++) {
		for (int x = -r; x <= r; x++) {
			int xx = x * x;
			int yy = y * y;
			if (xx + yy <= rr) {
				data.push_back({x0 + x, y0 + y});
			}
		}
	}
	setColor1();
	SDL_RenderDrawPoints(getRenderer(), data.data(), data.size());
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
