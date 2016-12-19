#pragma once
#include "Memory.h"
#include "PPURegisters.h"
#include "ColorPalette.h"
#include "SDLWrapper.h"
#include "Tile.h"
#include "Pixel.h"
#include <SDL.h>

class PPU {
	SDLWrapper SDL;

	unsigned short cScanline;
	unsigned short cCycle;

	// NES internal memory
	Memory* memory;

	// Rendering Data
	Tile tile;
	Pixel pixel;

	ColorPalette palette;

public:
	~PPU();

	bool *cpuNMI;

	void Initialize(Memory* memory, bool* cpuNMI);

	void EmulateCycle();
	void PreRender();
	void RenderScanline();
	void PostRender();
	void VBlank();

	// Data retrieval
	void LoadTile();
	void LoadPixel();
};