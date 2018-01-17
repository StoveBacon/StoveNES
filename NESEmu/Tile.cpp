#include "Tile.h"

void Tile::SetNametableIndex(unsigned short x, unsigned short y) {
	nametableIndex = (x / TileSize) + ((y / TileSize) * NumColumns);
	shiftIndex = 7;
}

void Tile::SetTileNumber(Memory* memory) {
	number = memory->ReadByteVRAM(0x2000 + nametableIndex);
}

void Tile::SetTileAddress(unsigned short y) {
	address = number * 0x10;
	address += (y % TileSize);
	address += 0x1000;
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

unsigned short Tile::GetPaletteIndex() {
	paletteIndex = ((pattern1 >> shiftIndex) & 0x01) + (((pattern2 >> shiftIndex) & 0x01) << 1);
	shiftIndex--;
	return paletteIndex;
}