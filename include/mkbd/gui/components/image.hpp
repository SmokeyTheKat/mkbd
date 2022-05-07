#ifndef __MKBD_GUI_IMAGE_HPP__
#define __MKBD_GUI_IMAGE_HPP__

#include <mkbd/gui/component.hpp>
#include <mkbd/gui/layout.hpp>
#include <mkbd/gui/texture.hpp>

#include <SDL2/SDL.h>

class ImageComponent : public Component {
	Texture mTexture;

public:
	ImageComponent(Layout layout, const char* path);
	void draw(void);
	void init(void);
	
private:
};

#endif