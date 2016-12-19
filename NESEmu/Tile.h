#pragma once
#include "Memory.h"

class Tile {
	const short TileSize = 8;
	const short NumColumns = 32;
	const short NumRows = 30;
	const short AttributeGridSize = 32;

	unsigned short nametableIndex;
	unsigned short number;
	unsigned short address;
	unsigned short pattern1;
	unsigned short pattern2;
	unsigned short paletteNumber;

public:
	void SetNametableIndex(unsigned short x, unsigned short y);
	void SetTileNumber(Memory* memory);
	void SetTileAddress(unsigned short y);
	void SetPatterns(Memory* memory);
	unsigned short GetPaletteIndex(unsigned short x, unsigned short y);
	void SetPaletteNumber(unsigned short x, unsigned short y, Memory* memory);
	unsigned short GetPaletteNumber();
};