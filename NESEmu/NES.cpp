#include "NES.h"
#include <iostream>

void NES::Initialize() {
	cpu.Initialize();
	cpu.Reset();
	ppu.Initialize(&cpu.NMI);
	controller.Initialize();
}

void NES::Run() {
	unsigned int cyclesUsed;
	unsigned int lastFrameTime = 0;
	unsigned int totalCycles = 0;
	for (;;) {
		if (SDLWrapper::Instance()->hasQuit) {
			return;
		}

		cyclesUsed = cpu.EmulateCycle();
		totalCycles += cyclesUsed;

		for (unsigned int i = 0; i < cyclesUsed * 3; i++) {
			ppu.EmulateCycle();
		}
		
		Debug::PrintFPS(&lastFrameTime, &totalCycles);
		Debug::LogCPUStatus(cpu.Status());

		controller.PollInput();
	}
}