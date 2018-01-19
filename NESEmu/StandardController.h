#pragma once
#include "SDLWrapper.h"
#include "Memory.h"

#define JOYPAD1 0x4016
#define JOYPAD2 0x4017

class StandardController {
	SDLWrapper *wrapper;
	Memory *memory;
	unsigned int keys;
public:
	void Initialize();
	void PollInput();
	unsigned int getNextBit();
};