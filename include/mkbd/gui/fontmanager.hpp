#ifndef __MKBD_GUI_FONTMANAGER_HPP__
#define __MKBD_GUI_FONTMANAGER_HPP__

#include <mkbd/utils.hpp>

#include <SDL_FontCache.h>
#include <SDL2/SDL.h>

namespace FontManager {
	FC_Font* getFont(const char* fontPath, int fontSize, Color color);
	void setRenderer(SDL_Renderer* renderer);
};

#endif