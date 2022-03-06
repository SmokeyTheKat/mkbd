#include <mkbd/gui/graphics/image.hpp>

ImageGraphic::ImageGraphic(Layout layout, const char* path)
: Graphic(layout), mTexture(path) {};

void ImageGraphic::init(void) {
	mTexture.setRenderer(getRenderer());
	mTexture.load();
}

void ImageGraphic::draw(void) {
	mTexture.render(mX, mY, mWidth, mHeight);
}
