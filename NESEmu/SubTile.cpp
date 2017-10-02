#include "SubTile.h"

Pixel *SubTile::PixelAt(int x) {
	return &pixels[x % 8];
}

void SubTile::InitializeSubTile(int x, int y) {
	this->x = x;
	this->y = y;
	this->memory = Memory::Instance();
	this->palette = ColorPalette::Instance();
	oldTile = new SubTile();
	SetNametableIndex();
}

/*
 * If this isn't the topmost subtile in a tile we can skip some calculations
 * Keep track of whether there has been a palette swap so skip calculations
 */
void SubTile::PopulateSubTile(bool paletteHasChanged) {
	SetOldTile();
	SetTileNumber();
	SetSubTileAddress();
	SetPaletteNumber();
	if (paletteHasChanged) {
		UpdatePixelColors();
	}
	// We only want to recalculate pixel data if the data for the pixels actually changed
	if (SetPatterns()) {
		PopulatePixels();
	}
}

void SubTile::SetNametableIndex() {
	nametableIndex = (x / TILE_SIZE) + ((y / TILE_SIZE) * NUM_COLUMNS);
}

void SubTile::SetTileNumber() {
	tileNumber = memory->ReadByteVRAM(NAMETABLE_0 + nametableIndex);
}

void SubTile::SetSubTileAddress() {
	tileAddress = tileNumber * 0x10;    // 0x10 is the width of one tile datum
	tileAddress += (y % TILE_SIZE);     // Because we're only manipulating a 1x8 section of an 8x8 tile we have to adjust for the y offset
	tileAddress += RIGHT_PATTERN_TABLE; // TODO: choose correct pattern table automatically
}

/*
 * This function selects which palette to use based off the x and y indexed into the attribute table.
 * From the 'tile' of data retrieved, the number must be extrapolated from 1 of the 4 quadrants that each tile of the
 * attribute table is seperated into. See https://goo.gl/XjfD2S for further reference.
*/
void SubTile::SetPaletteNumber() {
	unsigned short paletteByte;
	paletteByte = ATTRIBUTE_TABLE_0 + ((x / ATTRIBUTE_GRID_SIZE) % 8);
	paletteByte += (8 * (y / ATTRIBUTE_GRID_SIZE));
	paletteByte = memory->ReadByteVRAM(paletteByte);

	unsigned short quadrant;
	quadrant = ((x % ATTRIBUTE_GRID_SIZE) / 16);
	quadrant += (((y % ATTRIBUTE_GRID_SIZE) / 16)) << 1;
	paletteNumber = paletteByte >> ((quadrant) * 2);
	paletteNumber = paletteNumber & 0x03;
}

/*
 * If the patterns in memory match the current ones we don't need to bother with recalculating the palette indexes for each pixel.
 * Therefore, this function will return true if the patterns are different and false otherwise.
 */
bool SubTile::SetPatterns() {
	unsigned int tempPattern1, tempPattern2;
	tempPattern1 = memory->ReadByteVRAM(tileAddress);
	tempPattern2 = memory->ReadByteVRAM(tileAddress + TILE_SIZE);

	if (pattern1 == tempPattern1 && pattern2 == tempPattern2) {
		return false;
	} else {
		pattern1 = tempPattern1;
		pattern2 = tempPattern2;
		return true;
	}
}

// Make sure there's been no palette swaps
void SubTile::UpdatePixelColors() {
	for (int i = 0; i < 8; i++) {
		unsigned short colorByte = memory->ReadByteVRAM(PALETTE + pixels[i].paletteIndex + (paletteNumber * 4));
		pixels[i].color = &palette->colorMap[colorByte];
	}
}

void SubTile::PopulatePixels() {
	for (int i = 0; i < 8; i++) {
		pixels[i].x = this->x + i;
		pixels[i].y = this->y;
		pixels[i].paletteNumber = this->paletteNumber;
		// Shift the bit over to the right (if the bit is at x 0, it needs to move 7 to the right)
		pixels[i].paletteIndex = ((pattern1 >>  (7 - (pixels[i].x % 8))) & 0x01) + (((pattern2 >> ( 7 - (pixels[i].x % 8))) & 0x01) << 1);
		unsigned short colorByte = memory->ReadByteVRAM(PALETTE + pixels[i].paletteIndex + (paletteNumber * 4));
		pixels[i].color = &palette->colorMap[colorByte];
	}
}

void SubTile::SetOldTile() {
	oldTile->tileNumber = tileNumber;
	oldTile->tileAddress = tileAddress;
	oldTile->paletteNumber = paletteNumber;
	oldTile->pattern1 = pattern1;
	oldTile->pattern2 = pattern2;
}

bool SubTile::HasChanged() {
	// Compate the current tile to the old one
	if (oldTile->tileNumber != tileNumber ||
		oldTile->tileAddress != tileAddress ||
		oldTile->paletteNumber != paletteNumber ||
		oldTile->pattern1 != pattern1 ||
		oldTile->pattern2 != pattern2) {

		return true;
	}
	return false;
}