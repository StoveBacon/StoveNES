#pragma once
#include "Memory.h"
#include "PPURegisters.h"
#include "SDLWrapper.h"
#include "Tile.h"
#include "Pixel.h"
#include "Sprite.h"
#include "SubTile.h"
#include <SDL.h>

#define LIMIT_FRAMERATE false

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 240

// Rendering occurs in 4 distinct phases
#define PRERENDERING -1  // [-1, -1]
#define RENDERING 240    // [0, 240)
     // POSTRENDERING    // [240, 240]
#define VBLANKING 260    // (240, 260]

#define DRAWING_CYCLES 256
#define CYCLES_PER_LINE 340 // Inclusive

class PPU {
private:
	SDLWrapper *SDL;

	unsigned short currScanline, currCycle;
	SubTile *subTiles[32][240];

	Memory *memory;

	bool *cpuNMI;

	// Watch for palette swaps
	bool paletteHasChanged;
	bool useOldTile;

	// For frame limiting
	int lastFrameTime;

public:
	~PPU();

	void Initialize(bool *cpuNMI);
	void EmulateCycle();
	void PreRender();
	void RenderScanline();
	void PostRender();
	void VBlank();
};

//
//class PPU {
//	SDLWrapper *SDL;
//
//	unsigned short cScanline;
//	unsigned short cCycle;
//
//	// NES internal memory
//	Memory* memory;
//
//	// Rendering Data
//	Tile tile;
//	short lastTileNumber[32][300];
//	Pixel pixel;
//	unsigned short lastColorByte[300][300];
//
//	bool useLastTile;
//
//	ColorPalette palette;
//	unsigned short backgroundPalette[16];
//
//public:
//	~PPU();
//
//	bool *cpuNMI;
//
//	void Initialize(bool* cpuNMI);
//
//	void EmulateCycle();
//	void PreRender();
//	void RenderScanline();
//	void PostRender();
//	void VBlank();
//
//	// Data retrieval
//	void LoadTile();
//	void LoadPixel();
//
//	void DrawSprites();
//};
