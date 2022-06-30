#include <mkbd/gui/components/image.hpp>

ImageComponent::ImageComponent(Layout layout, const char* path)
: Component(layout), mTexture(path) {};

void ImageComponent::init(void) {
	mTexture.load();
}

void ImageComponent::draw(void) {
	mTexture.render(mX, mY, mWidth, mHeight);
}
