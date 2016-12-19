#include "Tile.h"

void Tile::SetNametableIndex(unsigned short x, unsigned short y) {
	nametableIndex = (x / TileSize) + ((y / TileSize) * NumColumns);
}

void Tile::SetTileNumber(Memory* memory) {
	number = memory->ReadByteVRAM(0x2000 + nametableIndex);
}

void Tile::SetTileAddress(unsigned short y) {
	address = number * 0x10;
	address += (y % TileSize);
	address += 0x1000; // Right pattern table
}

void Tile::SetPatterns(Memory* memory) {
	pattern1 = memory->ReadByteVRAM(address);
	pattern2 = memory->ReadByteVRAM(address + TileSize);
}

void Tile::SetPaletteNumber(unsigned short x, unsigned short y, Memory* memory) {
	unsigned short paletteByte;
	paletteByte = 0x23c0 + ((x / AttributeGridSize) % 8);
	paletteByte += (8 * (y / AttributeGridSize));
	paletteByte = memory->ReadByteVRAM(paletteByte);

	unsigned short quadrant;
	quadrant = ((x % AttributeGridSize) / 16);
	quadrant += (((y % AttributeGridSize) / 16)) << 1;
	paletteNumber = paletteByte >> ((quadrant) * 2);
	paletteNumber = paletteNumber & 0x03;
}

unsigned short Tile::GetPaletteNumber() {
	return paletteNumber;
}

unsigned short Tile::GetPaletteIndex(unsigned short x, unsigned short y) {
	unsigned short pixelSliver; // 8x1 pixel sliver from tile
	unsigned short paletteIndex;
	pixelSliver = pattern1;
	paletteIndex = pixelSliver >> (7 - (x % TileSize));
	paletteIndex = paletteIndex & 0x01;
	unsigned short bit2 = pattern2;
	bit2 = bit2 >> (7 - (x % TileSize));
	bit2 = (bit2 & 0x01) << 1;
	return paletteIndex += bit2;
}