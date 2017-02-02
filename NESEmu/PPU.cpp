#include "PPU.h"

void PPU::Initialize(Memory* memory, bool* cpuNMI, SDLWrapper *wrapper) {
	SDL = wrapper;

	// init SDL
	SDL->Initialize(256, 240, 2);

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
		SDL->DrawPixel(pixel.x, pixel.y, pixel.color);
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
		DrawSprites();
		SDL->UpdateWindowSurface();
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

void PPU::DrawSprites() {
	Sprite sprite;
	for (int i = 0; i < 0x100; i += 4) {
		sprite.y = memory->ReadByteOAM(i);
		sprite.tileIndex = (memory->ReadByteOAM(i + 1) & 0b11111110) >> 1;
		sprite.table = memory->ReadByteOAM(i + 1) & 0x01;
		unsigned short attributes = memory->ReadByteOAM(i + 2);
		sprite.flipVert = attributes & 0b10000000 >> 7;
		sprite.flipHoriz = attributes & 0b01000000 >> 6;
		sprite.priority = attributes & 0b00100000 >> 5;
		sprite.palette = attributes & 0b11;
		sprite.x = memory->ReadByteOAM(i + 3);
		unsigned short spriteAddr = sprite.tileIndex * 0x20;
		spriteAddr += sprite.table ? 0x10 : 0;

		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				unsigned short paletteIndex = memory->ReadByteVRAM(spriteAddr + j ) >> (7 - k);
				paletteIndex = paletteIndex & 0x01;
				paletteIndex += ((memory->ReadByteVRAM(spriteAddr + j + 8) >> (7 - k)) & 0x01) << 1;
				if (paletteIndex != 0)
				{
					SDL_Color color = palette.colorMap[memory->ReadByteVRAM(0x3F10 + paletteIndex + (sprite.palette * 4))];
					SDL->DrawPixel(sprite.x + k, sprite.y + j, color);
				}
			}
		}
		
	}
}

PPU::~PPU() {
	SDL->ShutDown();
}