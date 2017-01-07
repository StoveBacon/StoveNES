#pragma once
#include <SDL.h>

struct Pixel {
	unsigned short x, y;
	unsigned short paletteIndex;
	unsigned short paletteNumber;
	SDL_Color color;
};