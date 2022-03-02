#include <mkbd/gui/graphics/text.hpp>

#include <mkbd/gui/fontmanager.hpp>

TextGraphic::TextGraphic(std::string text, const char* fontPath, int fontSize, Color color, int x, int y, int width, int height)
: mText(text), mFontPath(fontPath), mFontSize(fontSize), mColor(color),
  Graphic(x, y, width, height) {};

void TextGraphic::draw(void) {
	FC_Font* font = FontManager::getFont(mFontPath, mFontSize, mColor);
	FC_Draw(font, mRenderer, mX, mY, mText.c_str()); 
}