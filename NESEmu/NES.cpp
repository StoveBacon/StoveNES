#include "NES.h"
#include <iostream>

void NES::Initialize() {
	cpu.Initialize();
	cpu.Reset();
	ppu.Initialize(&cpu.NMI);
	controller.Initialize();
}

void NES::Run() {
	unsigned short cyclesUsed;
	unsigned int lastFrameTime = 0;
	unsigned int totalCycles = 0;
	for (;;) {
		if (SDLWrapper::Instance()->hasQuit) {
			return;
		}

		cyclesUsed = cpu.EmulateCycle();
		totalCycles += cyclesUsed;

		for (int i = 0; i < cyclesUsed * 3; i++) {
			ppu.EmulateCycle();
		}
		// Print FPS
		if (SDL_GetTicks() > lastFrameTime + 1000) {
			printf("FPS: %i\n", ((totalCycles * 3) / 341) / 261);
			lastFrameTime = SDL_GetTicks();
			totalCycles = 0;
		}
		
		controller.PollInput();
	}
}