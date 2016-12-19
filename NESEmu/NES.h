#pragma once
#include "CPU.h";
#include "PPU.h";
#include "Memory.h";

class NES {
	CPU cpu;
	PPU ppu;

	Memory memory;

public:
	void Initialize();
	void Run();
};