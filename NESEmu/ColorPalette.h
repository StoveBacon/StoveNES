#include "SDL.h"

class ColorPalette {
public:
	ColorPalette() {
		SDL_Color color;

		colorMap[0x00] = Color(124, 124, 124);
		colorMap[0x10] = Color(188, 188, 188);
		colorMap[0x20] = Color(248, 248, 248);
		colorMap[0x30] = Color(252, 252, 252);

		colorMap[0x01] = Color(0, 0, 252);
		colorMap[0x11] = Color(0, 120, 248);
		colorMap[0x21] = Color(60, 188, 252);
		colorMap[0x31] = Color(164, 228, 252);

		colorMap[0x02] = Color(0, 0, 188);
		colorMap[0x12] = Color(0, 88, 248);
		colorMap[0x22] = Color(104, 136, 252);
		colorMap[0x32] = Color(184, 184, 248);

		colorMap[0x03] = Color(68, 40, 188);
		colorMap[0x13] = Color(104, 68, 252);
		colorMap[0x23] = Color(152, 120, 248);
		colorMap[0x33] = Color(216, 184, 248);

		colorMap[0x04] = Color(148, 0, 132);
		colorMap[0x14] = Color(216, 0, 204);
		colorMap[0x24] = Color(248, 120, 248);
		colorMap[0x34] = Color(248, 184, 248);

		colorMap[0x05] = Color(168, 0, 32);
		colorMap[0x15] = Color(228, 0, 88);
		colorMap[0x25] = Color(248, 88, 152);
		colorMap[0x35] = Color(248, 164, 192);

		colorMap[0x06] = Color(168, 16, 0);
		colorMap[0x16] = Color(248, 56, 0);
		colorMap[0x26] = Color(248, 120, 88);
		colorMap[0x36] = Color(240, 208, 176);

		colorMap[0x07] = Color(136, 20, 0);
		colorMap[0x17] = Color(228, 92, 16);
		colorMap[0x27] = Color(252, 160, 68);
		colorMap[0x37] = Color(252, 224, 168);

		colorMap[0x08] = Color(80, 48, 0);
		colorMap[0x18] = Color(172, 124, 0);
		colorMap[0x28] = Color(248, 184, 0);
		colorMap[0x38] = Color(248, 216, 120);

		colorMap[0x09] = Color(0, 120, 0);
		colorMap[0x19] = Color(0, 184, 0);
		colorMap[0x29] = Color(184, 248, 24);
		colorMap[0x39] = Color(216, 248, 120);

		colorMap[0x0A] = Color(0, 104, 0);
		colorMap[0x1A] = Color(0, 168, 0);
		colorMap[0x2A] = Color(88, 216, 84);
		colorMap[0x3A] = Color(184, 248, 184);

		colorMap[0x0B] = Color(0, 88, 0);
		colorMap[0x1B] = Color(0, 168, 68);
		colorMap[0x2B] = Color(88, 248, 152);
		colorMap[0x3B] = Color(184, 248, 216);

		colorMap[0x0C] = Color(0, 64, 88);
		colorMap[0x1C] = Color(0, 136, 136);
		colorMap[0x2C] = Color(0, 232, 216);
		colorMap[0x3C] = Color(0, 252, 252);

		colorMap[0x0D] = Color(0, 0, 0);
		colorMap[0x1D] = Color(0, 0, 0);
		colorMap[0x2D] = Color(120, 120, 120);
		colorMap[0x3D] = Color(248, 216, 248);

		SDL_Color black = Color(0, 0, 0);
		colorMap[0x0E] = black;
		colorMap[0x1E] = black;
		colorMap[0x2E] = black;
		colorMap[0x3E] = black;
		colorMap[0x0F] = black;
		colorMap[0x1F] = black;
		colorMap[0x2F] = black;
		colorMap[0x3F] = black;
	}

	SDL_Color Color(int r, int g, int b) {
		SDL_Color color;
		color.r = r;
		color.g = g;
		color.b = b;
		return color;
	}
	SDL_Color colorMap[64];
};