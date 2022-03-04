#ifndef __MKBD_TEXTURE_HPP__
#define __MKBD_TEXTURE_HPP__

#include <SDL2/SDL.h>

class Texture {
	const char* mPath;
	SDL_Texture* mTexture = 0;
	SDL_Renderer* mRenderer;
	int mWidth;
	int mHeight;
	int mX = 0;
	int mY = 0;
	bool loaded = false;

public:
	Texture(const char* path, SDL_Renderer* renderer);
	Texture(const char* path);
	~Texture(void);

	inline void setRenderer(SDL_Renderer* renderer) { mRenderer = renderer; };

	void render(void);
	void render(int x, int y);
	void render(int x, int y, double scale);
	void render(int x, int y, int width, int height);

	void scaleToWidth(int width);
	void scaleToHeight(int height);

	void load(void);

private:
	void free(void);
};


#endif