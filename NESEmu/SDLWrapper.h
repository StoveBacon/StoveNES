#pragma once
#include "FileManager.h"
#include <SDL.h>
#include <stdio.h>

class SDLWrapper {
	static SDLWrapper* instance;

	SDL_Window* window;
	SDL_Surface* screenSurface;
	SDL_Renderer* renderer;
	int screenWidth;
	int screenHeight;
	int upscale;

	SDL_Event event;

	// Double buffer for optimization
	SDL_Color *lastFrame[256][240];

	bool up, down, left, right, b, a, start, select;

public:
	static SDLWrapper* Instance();

	bool Initialize(int width, int height, int upscale);
	bool isNonTransparentPixel(int x, int y);
	void DrawPixel(unsigned short x, unsigned short y, SDL_Color *color);
	void UpdateWindowSurface();
	void ClearWindow();

	unsigned short GetKeys();

	bool hasQuit;

	void ShutDown();
};