#include "Sprite.h"

Sprite::Sprite() {
	pixels = new Pixel[8];

	this->memory = Memory::Instance();
	this->palette = ColorPalette::Instance();
}

// Read OAM and set all the values of the sprites
void Sprite::SetSpriteData(int spriteNumber) {
	unsigned short attributes, spriteMode;
	// Each sprite is 4 bytes wide, so multiply spriteNumber by this to get the OAM offset
	spriteNumber *= 4;

	this->y = memory->ReadByteOAM(SPRITE_Y_OFFSET + spriteNumber);
	this->tileIndex = (memory->ReadByteOAM(INDEX_TABLE_OFFSET + spriteNumber) & INDEX_MASK) >> 1;
	this->table = memory->ReadByteOAM(INDEX_TABLE_OFFSET + spriteNumber) & BANK_MASK;
	attributes = memory->ReadByteOAM(ATTRIBUTE_OFFSET + spriteNumber);
	this->flipHoriz = (attributes & FLIP_HORIZ_MASK) >> 7;
	this->flipVert = (attributes & FLIP_VERT_MASK) >> 6;
	this->priority = (attributes & PRIORITY_MASK) >> 5;
	this->paletteNumber = (attributes & PALETTE_MASK);
	this->x = memory->ReadByteOAM(SPRITE_X_OFFSET + spriteNumber);

	// Determine which pattern table to load from
	// 0 = 8x8 else 8x16
	this->height = (this->memory->ReadByte(PPUCTRL) & PPUCTRL::SPRITE_SIZE);
	int temp = this->memory->ReadByte(PPUCTRL);

	// If the sprite is 8x16 use the table from OAM, otherwise use the table selected from PPUCTRL
	spriteMode = (this->height) ? this->table : (this->memory->ReadByte(PPUCTRL) & PPUCTRL::SPRITE_TABLE);
	this->bottom = y + ((this->height) ? 0x10 : 0x08);
	this->address = (this->tileIndex * SPRITE_WIDTH) + ((spriteMode) ? RIGHT_PATTERN_TABLE : LEFT_PATTERN_TABLE);
	this->address += this->table ? 0x10 : 0; // TODO: Figure out why this works
}

Pixel *Sprite::RenderLine(int y) {
	unsigned short paletteIndex;

	// Make sure this sprite is actually on the line given
	if (y < this->y || y >= this->bottom) {
		return NULL;
	}

	// Make sure we account for vertical flips
	if (flipVert) {
		y = 7 - y;
	}

	// Mod by 8 to get local offset
	pattern1 = this->memory->ReadByteVRAM(address + (y % 8));
	pattern2 = this->memory->ReadByteVRAM(address + (y % 8) + 8);

	for (int i = 0; i < 8; i++) {
		Pixel pixel;
		pixel.x = this->x + i;
		pixel.y = y;
		pixel.paletteNumber = this->paletteNumber;
		paletteIndex = (pattern1 >> (7 - i)) & 0x01;
		paletteIndex += ((pattern2 >> (7 - i)) & 0x01) << 1;

		if (paletteIndex == 0) {
			pixel.color = &palette->transparent;
		} else {
			pixel.color = &palette->colorMap[memory->ReadByteVRAM(SPRITE_PALETTE + paletteIndex + (this->paletteNumber * 4))];
		}

		if (flipHoriz) {
			pixels[7 - i] = pixel;
		} else {
			pixels[i] = pixel;
		}
	}

	return pixels;
}