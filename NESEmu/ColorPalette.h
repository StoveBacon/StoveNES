#pragma once
#include "SDL.h"

class ColorPalette {
private:
	static ColorPalette *instance;
public:
	ColorPalette();

	SDL_Color Color(int r, int g, int b);

	static ColorPalette *Instance();
	SDL_Color colorMap[64];
};