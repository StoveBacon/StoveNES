#pragma once
#include <SDL.h>
#include <stdio.h>

class SDLWrapper {
	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Renderer* renderer;
	int screenWidth;
	int screenHeight;
	int upscale;

public:

	bool Initialize(int width, int height, int up);
	void DrawPixel(unsigned short x, unsigned short y, SDL_Color color);
	void UpdateWindowSurface();
	void ShutDown();
};