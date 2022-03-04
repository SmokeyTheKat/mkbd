#include <mkbd/gui/graphics/image.hpp>

ImageGraphic::ImageGraphic(int x, int y, int width, int height, const char* path)
: Graphic(x, y, width, height), mTexture(path) {};

void ImageGraphic::init(void) {
	mTexture.setRenderer(getRenderer());
	mTexture.load();
}

void ImageGraphic::draw(void) {
	mTexture.render(mX, mY, mWidth, mHeight);
}
