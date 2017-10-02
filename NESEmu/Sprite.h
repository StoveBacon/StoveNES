#pragma once

class Sprite {
public:
	unsigned short x, y;
	unsigned short tileIndex;
	unsigned short table, palette, priority, flipHoriz, flipVert;

	unsigned short pattern1[8];
	unsigned short pattern2[8];
};