#include <mkbd/gui/components/text.hpp>

#include <mkbd/gui/fontmanager.hpp>

#include <GL/gl.h>
#include "drawtext.h"

TextComponent::TextComponent(Layout layout, std::string text, const char* fontPath, int fontSize, Color color)
: Component(layout), mText(text), mFontPath(fontPath),
  mFontSize(fontSize), mColor(color) {};

void TextComponent::init(void) {
	generateFont();
};

void TextComponent::onResize(int width, int height) {
}

void TextComponent::generateFont(void) {
	if (!(mFont = dtx_open_font(mFontPath, mFontSize))) {
		fprintf(stderr, "failed to open font\n");
		return;
	}
	dtx_use_font(mFont, mFontSize);
}


void TextComponent::draw(void) {
	dtx_use_font(mFont, mFontSize);

	struct dtx_box box;
	dtx_string_box(mText.c_str(), &box);

	float y = 0;
	float x = 0;

	switch (mHAlign) {
		case HAlign::Left: {
			x = mX;
		} break;
		case HAlign::Right: {
			x = mX + mWidth - box.width;
		} break;
		case HAlign::Middle: {
			x = mX + mWidth / 2.0 - box.width / 2.0;
		} break;
	}

	switch (mVAlign) {
		case VAlign::Top: {
			y = mY + box.height;
		} break;
		case VAlign::Bottom: {
			y = mY + mHeight;
		} break;
		case VAlign::Middle: {
			y = mY + mHeight / 2.0 + box.height / 2.0;
		} break;
	}

	glPushMatrix();

	glTranslatef(x, y, 0);
	glScalef(1,-1,1);
	glColor3ub(RGB_ARGS(mColor));

	dtx_string(mText.c_str());

	glPopMatrix();
}
