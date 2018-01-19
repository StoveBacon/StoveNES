#pragma once
#include "Memory.h"
#include "PPURegisters.h"
#include "SDLWrapper.h"
#include "Pixel.h"
#include "Sprite.h"
#include "SubTile.h"
#include <SDL.h>

#define LIMIT_FRAMERATE true
#define FRAMERATE 60
#define TILE_DRAW_SKIP false

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
	Sprite sprites[64];

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