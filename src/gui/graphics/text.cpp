#include <mkbd/gui/graphics/text.hpp>

#include <mkbd/gui/fontmanager.hpp>

TextGraphic::TextGraphic(int x, int y, int width, int height, std::string text, const char* fontPath, int fontSize, Color color)
: Graphic(x, y, width, height), mText(text), mFontPath(fontPath),
  mFontSize(fontSize), mColor(color) {};

void TextGraphic::init(void) {}

void TextGraphic::draw(void) {
	FC_Font* font = FontManager::getFont(mFontPath, mFontSize, mColor);
	FC_Draw(font, getRenderer(), mX, mY, mText.c_str()); 
}
