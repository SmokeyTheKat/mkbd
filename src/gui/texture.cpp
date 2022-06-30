#include <mkbd/gui/texture.hpp>

#include <SDL2/SDL_image.h>

#include <iostream>

Texture::Texture(std::string path)
: mPath(path) { load(path); };

Texture::~Texture(void) { free(); };

void Texture::render(void) {
	render(mX, mY, mWidth, mHeight);
}

void Texture::render(int x, int y) {
	render(x, y, mWidth, mHeight);
}

void Texture::render(int x, int y, double scale) {
	render(x, y, (double)mWidth * scale, (double)mHeight * scale);
}

void Texture::render(int x, int y, int width, int height) {
//    SDL_Rect rect;
//    rect.x = x;
//    rect.y = y;
//    rect.w = width;
//    rect.h = height;
//
//    SDL_RenderCopy(mRenderer, mTexture, 0, &rect);


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, mTexture);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex2i(x, y);
		glTexCoord2i(1, 0); glVertex2i(x + width, y);
		glTexCoord2i(1, 1); glVertex2i(x + width, y + height);
		glTexCoord2i(0, 1); glVertex2i(x, y + height);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	
}

void Texture::scaleToWidth(int width) {
	double scale = (double)width / (double)mWidth;
	mWidth = width;
	mHeight *= scale;
}

void Texture::scaleToHeight(int height) {
	double scale = (double)height / (double)mHeight;
	mHeight = height;
	mWidth *= scale;
}

void Texture::load(void) {
	load(mPath);
}

void Texture::load(std::string path) {
	mPath = path;

	free();
	mSurface = IMG_Load(path.c_str());
	
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &mTexture);
	glBindTexture(GL_TEXTURE_2D, mTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLenum format;
	if (mSurface->format->BytesPerPixel == 4) {
		if (mSurface->format->Rmask == 0x000000ff) {
			format = GL_RGBA;
		} else {
			format = GL_BGRA;
		}
	}
	else if (mSurface->format->BytesPerPixel == 3) {
		if (mSurface->format->Rmask == 0x0000ff) {
			format = GL_RGB;
		} else {
			format = GL_BGR;
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, mSurface->format->BytesPerPixel, mSurface->w, mSurface->h, 0, format, GL_UNSIGNED_BYTE, mSurface->pixels);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);

	loaded = true;
}

void Texture::free(void) {
	loaded = false;
	if (mSurface == 0)
		return;

//    glBindTexture(mTexture, 0);
//    glDeleteTextures(1, &mTexture);
//    glBindTexture(GL_TEXTURE_2D, 0);

//    SDL_FreeSurface(mSurface);
	mSurface = 0;
}
