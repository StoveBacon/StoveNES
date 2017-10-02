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

//
//void PPU::Initialize(bool* cpuNMI) {
//	this->SDL = SDLWrapper::Instance();
//
//	this->cScanline = 241;
//	this->cCycle = 0;
//
//	this->memory = Memory::Instance();
//	this->cpuNMI = cpuNMI;
//}
//
//void PPU::EmulateCycle() {
//	if (cScanline == -1) {
//		PreRender();
//
//	} else if (cScanline < 240) {
//		RenderScanline();
//
//	} else if (cScanline == 240) {
//		PostRender();
//
//	} else if (cScanline <= 260) {
//		VBlank();
//
//	} else {
//		cScanline = 0;
//	}
//}
//
//void PPU::PreRender() {
//	// PPU is fetching bytes, but this currently is not emulated so just advance the cycles
//	if (cCycle <= 340) {
//		cCycle++;
//	} else {
//		cCycle = 0;
//		cScanline++;
//	}
//}
//
//void PPU::RenderScanline() {
//	if (cCycle == 0) {
//		LoadTile();
//		cCycle++;
//		return;
//
//	} else if (cCycle <= 256) {
//		// Internally sets all needed variables
//		if (cCycle % 8 == 0) {
//			LoadTile();
//		}
//		if (useLastTile) {
//			SDL->DrawPixel(cCycle, cScanline, palette.colorMap[lastColorByte[cCycle][cScanline]]);
//		} else {
//			LoadPixel();
//			SDL->DrawPixel(pixel.x, pixel.y, pixel.color);
//		}
//		cCycle++;
//
//	} else if (cCycle <= 340) {
//		// Data retrievel. For this iteration, these will be idle.
//		cCycle++;
//		return;
//
//	} else {
//		// Reset cycles and advance to next scanline
//		cCycle = 0;
//		cScanline++;
//	}
//}
//
//void PPU::PostRender() {
//	// Update the window surface. Cheat currently by calling prerender
//	if (cCycle == 0) {
//		DrawSprites();
//		SDL->UpdateWindowSurface();
//	}
//	PreRender();
//}
//
//// Set PPU VBlank flag to trigger NMI in the CPU
//void PPU::VBlank() {
//	// Set on second cycle of VBlank
//	if (cScanline == 241 && cCycle == 1) {
//		memory->WriteByte(PPUSTATUS, PPUSTATUS::VBLANK);
//		*cpuNMI = true;
//	}
//
//	if (cCycle <= 340) {
//		cCycle++;
//	} else {
//		memory->WriteByte(PPUSTATUS, 0);
//		*cpuNMI = false;
//		cCycle = 0;
//		cScanline++;
//	}
//}
//
//void PPU::LoadTile() {
//	tile.SetNametableIndex(cCycle, cScanline);
//	tile.SetTileNumber(memory);
//	tile.SetTileAddress(cScanline);
//	tile.SetPatterns(memory);
//	tile.SetPaletteNumber(cCycle, cScanline, memory);
//
//	unsigned short lastTileNum = lastTileNumber[cCycle / 8][cScanline];
//	if (lastTileNum != NULL) {
//		if (lastTileNum == tile.number) {
//			useLastTile = true;
//		} else {
//			lastTileNumber[cCycle / 8][cScanline] = tile.number;
//			useLastTile = false;
//		}
//	} else {
//		lastTileNumber[cCycle / 8][cScanline] = tile.number;
//		useLastTile = false;
//	}
//}
//
//void PPU::LoadPixel() {
//	pixel.x = cCycle;
//	pixel.y = cScanline;
//	pixel.paletteIndex = tile.GetPaletteIndex();
//	unsigned short colorByte = memory->ReadByteVRAM(0x3F00 + pixel.paletteIndex + (tile.paletteNumber * 4));
//	lastColorByte[pixel.x][pixel.y] = colorByte;
//	pixel.color = palette.colorMap[colorByte];
//}
//
//void PPU::DrawSprites() {
//	Sprite sprite;
//	for (int i = 0; i < 0x100; i += 4) {
//		sprite.y = memory->ReadByteOAM(i);
//		sprite.tileIndex = (memory->ReadByteOAM(i + 1) & 0b11111110) >> 1;
//		sprite.table = memory->ReadByteOAM(i + 1) & 0x01;
//		unsigned short attributes = memory->ReadByteOAM(i + 2);
//		sprite.flipVert = attributes & 0b10000000 >> 7;
//		sprite.flipHoriz = attributes & 0b01000000 >> 6;
//		sprite.priority = attributes & 0b00100000 >> 5;
//		sprite.palette = attributes & 0b11;
//		sprite.x = memory->ReadByteOAM(i + 3);
//		unsigned short spriteAddr = sprite.tileIndex * 0x20;
//		spriteAddr += sprite.table ? 0x10 : 0;
//
//		for (int j = 0; j < 8; j++) {
//			for (int k = 0; k < 8; k++) {
//				unsigned short paletteIndex = memory->ReadByteVRAM(spriteAddr + j ) >> (7 - k);
//				paletteIndex = paletteIndex & 0x01;
//				paletteIndex += ((memory->ReadByteVRAM(spriteAddr + j + 8) >> (7 - k)) & 0x01) << 1;
//				if (paletteIndex != 0)
//				{
//					SDL_Color color = palette.colorMap[memory->ReadByteVRAM(0x3F10 + paletteIndex + (sprite.palette * 4))];
//					SDL->DrawPixel(sprite.x + k, sprite.y + j, color);
//				}
//			}
//		}
//		
//	}
//}
//
//PPU::~PPU() {
//	SDL->ShutDown();
//}
