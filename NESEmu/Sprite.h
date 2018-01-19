#pragma once
#include "Memory.h"
#include "ColorPalette.h"
#include "Pixel.h"
#include <iterator>

// Byte 0 is just the y value of the sprite
#define SPRITE_Y_OFFSET 0x00

// Byte 1 contains the tile number and which tile bank to read from
#define INDEX_TABLE_OFFSET 0x01
#define INDEX_MASK 0xFE
#define BANK_MASK 0x01

// Byte 2 contains the attributes of the sprite
#define ATTRIBUTE_OFFSET 0x02
#define FLIP_VERT_MASK 0x80
#define FLIP_HORIZ_MASK 0x40
#define PRIORITY_MASK 0x20
#define PALETTE_MASK 0x03

// Byte 3 is just the x value of the sprite
#define SPRITE_X_OFFSET 0x03

// Used for keeping track of sprites (sprites are composed of 2 sets of 16 byte data)
#define SPRITE_WIDTH 0x20

class Sprite {
private:
	unsigned short x, y, bottom;
	unsigned short tileIndex, table, paletteNumber, priority, flipHoriz, flipVert, height, address;

	unsigned short pattern1;
	unsigned short pattern2;

	Pixel *pixels;

	Memory *memory;
	ColorPalette *palette;
public:
	Sprite();
	void SetSpriteData(int spriteNumber);
	Pixel *RenderLine(int y);
};