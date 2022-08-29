#include <mkbd/gui/component.hpp>

#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#include <mkbd/gui/window.hpp>

#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>


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
	glColor3ub(RGB_ARGS(mColor1));
}

void Component::setColor2(int r, int g, int b) {
	mColor2 = Color(r, g, b);
}

void Component::setColor2(void) {
	glColor3ub(RGB_ARGS(mColor2));
}

void Component::setColor(int r, int g, int b) {
	setColor1(r, g, b);
	setColor2(r, g, b);
}

void Component::drawLine(int x1, int y1, int x2, int y2) {
	setColor1();
	glBegin(GL_LINES);
		glVertex2i(mX + x1, mY + y1);
		glVertex2i(mX + x2, mY + y2);
	glEnd();
}

void Component::drawPoint(int x, int y) {
//    setColor1();
//    SDL_RenderDrawPoint(getRenderer(), mX + x, mY + y);
}

void Component::fillRoundedRectangle(int x, int y, int w, int h, int r) {
	if (2 * r > w || 2 * r > h) {
		int m = std::min(w, h);
		r = m / 2;
	}
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	fillRectangle(x+r, y, w - 2 * r, h);
	fillRectangle(x, y+r, w, h - 2 * r);
	fillCircle(x + r,     y + r,     r);
	fillCircle(x + w - r, y + r,     r);
	fillCircle(x + w - r, y + h - r, r);
	fillCircle(x + r,     y + h - r, r);
}

void Component::drawRoundedRectangle(int x, int y, int w, int h, int r) {
	drawRectangle(x, y, w, h);
}


void Component::fillRectangle(int x, int y, int w, int h) {
	setColor1();
	glRecti(
		mX + x, mY + y,
		mX + x + w, mY + y + h
	);
}

void Component::drawRectangle(int x, int y, int w, int h) {
	setColor2();
	glBegin(GL_LINES);
		glVertex2i(mX + x,     mY + y);
		glVertex2i(mX + x + w, mY + y);

		glVertex2i(mX + x + w, mY + y);
		glVertex2i(mX + x + w, mY + y + h);

		glVertex2i(mX + x + w, mY + y + h);
		glVertex2i(mX + x,     mY + y + h);

		glVertex2i(mX + x,     mY + y + h);
		glVertex2i(mX + x,     mY + y);
	glEnd();
}

void Component::drawRectangleWithOutline(int x, int y, int w, int h) {
	fillRectangle(x, y, w, h);
	drawRectangle(x, y, w, h);
}

void Component::drawCircle(int x0, int y0, int r, int t) {
	x0 += mX;
	y0 += mY;
	int num_segments = 10;

	float theta = M_PI * 2.0 / (float)num_segments;
	float tangetial_factor = tanf(theta);

	float radial_factor = cosf(theta);

	float x = r;

	float y = 0;
	glLineWidth(1);

	glBegin(GL_LINE_LOOP);
	for (int ii = 0; ii < num_segments; ii++)
	{
		glVertex2f(x + x0, y + y0);

		float tx = -y;
		float ty = x;

		x += tx * tangetial_factor;
		y += ty * tangetial_factor;

		x *= radial_factor;
		y *= radial_factor;
	}
	glEnd();
}

void Component::fillCircle(int x, int y, int r) {

	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(mX + x, mY + y);
		for (float i = 0; i <= 360; i++) {
			glVertex2f(r * cos(M_PI * i / 180.0) + mX + x, r*sin(M_PI * i / 180.0) + mY + y);
		}
	glEnd();
	drawCircle(x, y, r);
}

void Component::drawEllipse(int x, int y, int a0, int a1, int rx, int ry) {
	double i, inc;
	glBegin(GL_LINE_STRIP);
	glColor3f(1,1,0);
	inc=3.14/std::max(rx,ry)/8.0;
	i=a0;
	while(i<=a1)
	{
		double v = std::pow(cos(i + M_PI / 3.0), 2) + 1.0;
		glVertex2f(int((rx*cos(i)+y)+.5),int((ry*sin(i)+x)+.5));
		i+=v;
		glVertex2f(int((rx*cos(i)+y)+.5),int((ry*sin(i)+x)+.5));
		i-=v;
		i+=inc;
	}
	glEnd();
}

void Component::fillEllipse(int x0, int y0, int w, int h) {
}
