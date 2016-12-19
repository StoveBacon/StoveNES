#pragma once

// PPU register locations
const short PPUCTRL = 0x2000;
const short PPUMASK = 0x2001;
const short PPUSTATUS = 0x2002;
const short OAMADDR = 0x2003;
const short OAMDATA = 0x2004;
const short PPUSCROLL = 0x2005;
const short PPUADDR = 0x2006;
const short PPUDATA = 0x2007;
const short OAMDMA = 0x4014;

// PPU Register bitmasks

enum PPUCTRL { // 0x2000
	NMI_ENABLE = 0x80,
	PPU_MS = 0x40,
	SPRITE_HEIGHT = 0x20,
	BG_TILE_SELECT = 0x10,
	SPRITE_TILE_SELECT = 0x08,
	INCREMENT_MODE = 0x04,
	NAMETABLE_SELECT = 0x03 // 2 bits
};

enum PPUMASK { // 0x2001
	COLOR_EMPH_B = 0x80,
	COLOR_EMPH_G = 0x40,
	COLOR_EMPH_R = 0x20,
	SPRITE_ENABLE = 0x10,
	BACKGROUND_ENABLE = 0x08,
	SPRITE_LC_ENABLE = 0x04,
	BACKGROUND_LC_ENABLE = 0x02,
	GREYSCALE = 0x01
};

enum PPUSTATUS { // 0x2002
	VBLANK = 0x80,
	SPRITE_0_HIT = 0x40,
	SPRITE_OVERFLOW = 0x20
};