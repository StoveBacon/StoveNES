#pragma once
#include "Memory.h"
#include "ColorPalette.h"
#include "Pixel.h"

// Consants for tiles
#define TILE_SIZE 8
#define NUM_COLUMNS 32
#define NUM_ROWS 30
#define ATTRIBUTE_GRID_SIZE 32

class SubTile {
private:
	Pixel pixels[8];
	unsigned int x, y;
	unsigned int patternTableIndex, nametableIndex;
	unsigned int paletteNumber;
	unsigned int tileNumber, tileAddress;
	unsigned int pattern1, pattern2;

	// Keep a pointer to memory so we don't have to pass it all the time
	Memory *memory;

	ColorPalette *palette;

	// Helper functions
	void SetNametableIndex();
	void SetTileNumber();
	void SetSubTileAddress();
	void SetPaletteNumber();
	bool SetPatterns();
	void UpdatePixelColors();
	void PopulatePixels();
	void SetOldTile();

	// For seeing if the tile has changed
	SubTile *oldTile;
public:
	void InitializeSubTile(int x, int y);
	void PopulateSubTile(bool paletteHasChanged);
	Pixel *PixelAt(int x);

	// Keep track of whether the tile has changed. If it hasn't we don't have to bother rendering it.
	bool HasChanged();

	// Don't bother recalculating indexes or tile numbers
	unsigned int GetNextSubTileAddress();
};