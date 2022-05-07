#include <mkbd/gui/components/text.hpp>

#include <mkbd/gui/fontmanager.hpp>

TextComponent::TextComponent(Layout layout, std::string text, const char* fontPath, int fontSize, Color color)
: Component(layout), mText(text), mFontPath(fontPath),
  mFontSize(fontSize), mColor(color) {};

void TextComponent::init(void) {}

void TextComponent::draw(void) {
	FC_Font* font = FontManager::getFont(mFontPath, mFontSize, mColor);
	switch (mAlign) {
		case FC_ALIGN_LEFT: {
			FC_DrawAlign(font, getRenderer(), mX, mY, mAlign, mText.c_str()); 
		} break;
		case FC_ALIGN_RIGHT: {
			FC_DrawAlign(font, getRenderer(), mX + mWidth, mY, mAlign, mText.c_str()); 
		} break;
		case FC_ALIGN_CENTER: {
			FC_DrawAlign(font, getRenderer(), mX + mWidth / 2, mY, mAlign, mText.c_str()); 
		} break;
	}
}
