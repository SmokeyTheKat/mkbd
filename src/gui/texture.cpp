#include <mkbd/gui/texture.hpp>

#include <SDL2/SDL_image.h>

#include <iostream>

Texture::Texture(const char* path, SDL_Renderer* renderer)
: mPath(path), mRenderer(renderer) { load(); };

Texture::Texture(const char* path)
: mPath(path), mRenderer(0) { };

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
	SDL_Rect rect;
	rect.x = x;
	rect.y = y;
	rect.w = width;
	rect.h = height;

	SDL_RenderCopy(mRenderer, mTexture, 0, &rect);
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
	if (mRenderer == 0)
		return;

	free();
	SDL_Surface* surface = IMG_Load(mPath);
	mTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
	mWidth = surface->w;
	mHeight = surface->h;
	SDL_FreeSurface(surface);

	loaded = true;
}

void Texture::free(void) {
	loaded = false;
	if (mTexture == 0)
		return;

	SDL_DestroyTexture(mTexture);
	mTexture = 0;
}