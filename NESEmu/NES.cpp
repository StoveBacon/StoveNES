#include "NES.h"
#include <iostream>

void NES::Initialize() {
	cpu.Initialize(&memory);
	cpu.Reset();
	ppu.Initialize(&memory, &cpu.NMI, &wrapper);
	controller.Initialize(&memory, &wrapper);
}

void NES::Run() {
	unsigned short cyclesUsed;
	unsigned int lastTime = 0;
	unsigned int totalCycles = 0;
	for (;;) {
		cyclesUsed = cpu.EmulateCycle();
		totalCycles += cyclesUsed;

		for (int i = 0; i < cyclesUsed * 3; i++) {
			ppu.EmulateCycle();
		}
		if (SDL_GetTicks() > lastTime + 1000) {
			printf("FPS: %i\n", ((totalCycles * 3) / 341) / 261);
			lastTime = SDL_GetTicks();
			totalCycles = 0;
		}
		controller.PollInput();
	}
}