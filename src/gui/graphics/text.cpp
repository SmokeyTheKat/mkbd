#include <mkbd/gui/graphics/text.hpp>

#include <mkbd/gui/fontmanager.hpp>

TextGraphic::TextGraphic(Layout layout, std::string text, const char* fontPath, int fontSize, Color color)
: Graphic(layout), mText(text), mFontPath(fontPath),
  mFontSize(fontSize), mColor(color) {};

void TextGraphic::init(void) {}

void TextGraphic::draw(void) {
	FC_Font* font = FontManager::getFont(mFontPath, mFontSize, mColor);
	FC_DrawAlign(font, getRenderer(), mX + mWidth / 2, mY, mAlign, mText.c_str()); 
}
