#pragma once
#include "CPU.h";
#include "PPU.h";
#include "StandardController.h"
#include "Memory.h";
#include "SDLWrapper.h"

class NES {
	CPU cpu;
	PPU ppu;
	StandardController controller;

	Memory memory;

	SDLWrapper wrapper;

public:
	void Initialize();
	void Run();
};