#include <mkbd/gui/fontmanager.hpp>
#include <mkbd/utils.hpp>

#include <SDL2/SDL.h>

#include <vector>
#include <iostream>
#include <unordered_map>

struct FontDetails {
	const char* fontPath;
	int fontSize;
	Color color;
	inline FontDetails(const char* fontPath, int fontSize, Color color)
	: fontPath(fontPath), fontSize(fontSize), color(color) {};
	bool operator==(const FontDetails& other) const {
		return (
			fontPath == other.fontPath &&
			fontSize == other.fontSize &&
			color == other.color
		);
	}
};

namespace std {
	template <>
	struct hash<FontDetails> {
		std::size_t operator()(const FontDetails& fd) const {
			return ((hash<const char*>()(fd.fontPath) ^ (hash<int>()(fd.fontSize) << 1)) >> 1) &
					(hash<int>()(fd.color.r * fd.color.g * fd.color.b + fd.color.r + - fd.color.g - fd.color.b) << 1);
		}
	};
}

static std::unordered_map<FontDetails, FC_Font*> fontMap;
static SDL_Renderer* gRenderer;

static void registerFont(const FontDetails& fd) {
	FC_Font* font = FC_CreateFont();
	FC_LoadFont(font, gRenderer, fd.fontPath, fd.fontSize, FC_MakeColor(RGB_ARGS(fd.color), 255), TTF_STYLE_NORMAL);
	fontMap[fd] = font;
}

FC_Font* FontManager::getFont(const char* fontPath, int fontSize, Color color) {
	FontDetails fd(fontPath, fontSize, color);
	if (fontMap.find(fd) == fontMap.end()) {
		registerFont(fd);
	}
	return fontMap[fd];
}

void FontManager::setRenderer(SDL_Renderer* renderer) {
	gRenderer = renderer;
}
