#include "PPU.h"

void PPU::Initialize(bool* cpuNMI) {
	this->SDL = SDLWrapper::Instance();

	this->currScanline = 241;
	this->currCycle = 0;

	this->memory = Memory::Instance();
	this->cpuNMI = cpuNMI;

	this->lastFrameTime = 0;

	// Init the tiles
	for (int x = 0; x < SCREEN_WIDTH / 8; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			subTiles[x][y] = new SubTile();
			subTiles[x][y]->InitializeSubTile(x * 8, y);
		}
	}
}

/*
 * Check what scanline is being emulated and call the appropriate function.
 * Many functions are empty but may be implemented later to improve accuracy.
*/
void PPU::EmulateCycle() {
	if (currScanline == PRERENDERING) {
		PreRender();
	} else if (currScanline < RENDERING) {
		RenderScanline();
	} else if (currScanline == RENDERING) {
		PostRender();
	} else if (currScanline < VBLANKING) {
		VBlank();
	} else {
		currScanline = 0;
	}
}

// PPU normally is fetching memory, which is currently not emulated.
void PPU::PreRender() {
	if (currCycle <= CYCLES_PER_LINE) {
		currCycle += 1;
	} else {
		currCycle = 0;
		currScanline += 1;
	}
}

void PPU::RenderScanline() {
	if (currCycle < DRAWING_CYCLES) {
		// Only load a tile once
		if (currCycle % 8 == 0) {
			// Only modify the paletteHasChanged flag is a change hasn't already occurred.
			if (paletteHasChanged == false) {
				paletteHasChanged = memory->PaletteHasChanged();
			}

			subTiles[currCycle / 8][currScanline]->PopulateSubTile(paletteHasChanged);

			// If the subtile hasn't changed and there hasn't been a palette swap then we don't have to render anything.
			if (subTiles[currCycle / 8][currScanline]->HasChanged() || paletteHasChanged) {
				useOldTile = false;
			} else {
				useOldTile = true;
			}
		}
		if (!useOldTile) {
			SDL->DrawPixel(currCycle, currScanline, subTiles[currCycle / 8][currScanline]->PixelAt(currCycle % 8)->color);
		}
		currCycle += 1;
	} else if (currCycle <= CYCLES_PER_LINE) {
		// PPU emulation is idle here
		currCycle += 1;
		return;
	} else {
		// We've reached the end of this line
		currCycle = 0;
		currScanline += 1;
	}
}

void PPU::PostRender() {
	// Update the window surface. Cheat currently by calling prerender
	if (currCycle == 0) {
		SDL->UpdateWindowSurface();
		paletteHasChanged = false;
	}
	if (currCycle == SCREEN_WIDTH && LIMIT_FRAMERATE) {
		int t = SDL_GetTicks();
		if (SDL_GetTicks() - lastFrameTime < 1000 / 60) {
			SDL_Delay((1000 / 60) - (SDL_GetTicks() - lastFrameTime));
		}
		lastFrameTime = SDL_GetTicks();
	}

	PreRender();
}

// Set PPU VBlank flag to trigger NMI in the CPU
void PPU::VBlank() {
	// Set on second cycle of VBlank
	if (currScanline == 241 && currCycle == 1) {
		memory->WriteByte(PPUSTATUS, PPUSTATUS::VBLANK);
		*cpuNMI = true;
	}

	if (currCycle <= CYCLES_PER_LINE) {
		currCycle += 1;
	} else {
		memory->WriteByte(PPUSTATUS, 0);
		*cpuNMI = false;
		currCycle = 0;
		currScanline++;
	}
}

PPU::~PPU() {
	SDL->ShutDown();
	
	// Free subtiles
	for (int x = 0; x < SCREEN_WIDTH / 8; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			delete subTiles[x][y];
		}
	}
}