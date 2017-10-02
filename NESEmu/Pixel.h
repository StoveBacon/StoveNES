#pragma once
#include <SDL.h>

struct Pixel {
	unsigned short x, y;
	unsigned short paletteNumber;
	unsigned short paletteIndex;
	SDL_Color *color;
};