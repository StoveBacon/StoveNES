#pragma once
#include "Memory.h"

class Tile {
	const short TileSize = 8;
	const short NumColumns = 32;
	const short NumRows = 30;
	const short AttributeGridSize = 32;

	unsigned short nametableIndex;
	unsigned short address;
	unsigned short pattern1;
	unsigned short pattern2;
	unsigned short shiftIndex;

	// Temporary storage variables
	unsigned short pixelSliver;
	unsigned short paletteIndex;

	unsigned short paletteMemoryValue;

public:
	unsigned short number;
	unsigned short paletteNumber;

	void SetNametableIndex(unsigned short x, unsigned short y);
	void SetTileNumber(Memory* memory);
	void SetTileAddress(unsigned short y);
	void SetPatterns(Memory* memory);
	unsigned short GetPaletteIndex();
	void SetPaletteNumber(unsigned short x, unsigned short y, Memory* memory);
};