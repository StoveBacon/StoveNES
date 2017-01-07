#include "PPU.h"

void PPU::Initialize(Memory* memory, bool* cpuNMI) {
	// init SDL
	SDL.Initialize(256, 240, 1);

	cScanline = 241;
	cCycle = 0;

	this->memory = memory;
	this->cpuNMI = cpuNMI;
}

void PPU::EmulateCycle() {
	if (cScanline == -1) {
		PreRender();

	} else if (cScanline < 240) {
		RenderScanline();

	} else if (cScanline == 240) {
		PostRender();

	} else if (cScanline <= 260) {
		VBlank();

	} else {
		cScanline = 0;
	}
}

void PPU::PreRender() {
	// PPU is fetching bytes, but this currently is not emulated so just advance the cycles
	if (cCycle <= 340) {
		cCycle++;
	} else {
		cCycle = 0;
		cScanline++;
	}
}

void PPU::RenderScanline() {
	if (cCycle == 0) {
		LoadTile();
		cCycle++;
		return;

	} else if (cCycle <= 256) {
		// Internally sets all needed variables
		if (cCycle % 8 == 0) {
			LoadTile();
		}
		LoadPixel();
		SDL.DrawPixel(pixel.x, pixel.y, pixel.color);

		cCycle++;

	} else if (cCycle <= 340) {
		// Data retrievel. For this iteration, these will be idle.
		cCycle++;
		return;

	} else {
		// Reset cycles and advance to next scanline
		cCycle = 0;
		cScanline++;
	}
}

void PPU::PostRender() {
	// Update the window surface. Cheat currently by calling prerender
	if (cCycle == 0) {
		SDL.UpdateWindowSurface();
	}
	PreRender();
}

// Set PPU VBlank flag to trigger NMI in the CPU
void PPU::VBlank() {
	// Set on second cycle of VBlank
	if (cScanline == 241 && cCycle == 1) {
		memory->WriteByte(PPUSTATUS, PPUSTATUS::VBLANK);
		*cpuNMI = true;
	}

	if (cCycle <= 340) {
		cCycle++;
	} else {
		memory->WriteByte(PPUSTATUS, 0);
		*cpuNMI = false;
		cCycle = 0;
		cScanline++;
	}
}

void PPU::LoadTile() {
	tile.SetNametableIndex(cCycle, cScanline);
	tile.SetTileNumber(memory);
	tile.SetTileAddress(cScanline);
	tile.SetPatterns(memory);
	tile.SetPaletteNumber(cCycle, cScanline, memory);
}

void PPU::LoadPixel() {
	pixel.x = cCycle;
	pixel.y = cScanline;
	pixel.paletteIndex = tile.GetPaletteIndex();
	unsigned short colorByte = memory->ReadByteVRAM(0x3F00 + pixel.paletteIndex + (tile.paletteNumber * 4));
	pixel.color = palette.colorMap[colorByte];
}

PPU::~PPU() {
	SDL.ShutDown();
}