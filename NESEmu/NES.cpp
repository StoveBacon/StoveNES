#include "NES.h"

void NES::Initialize() {
	cpu.Initialize(&memory);
	cpu.Reset();
	ppu.Initialize(&memory, &cpu.NMI);
}

void NES::Run() {
	for (;;) {
		short cyclesUsed = cpu.EmulateCycle();

		for (int i = 0; i < cyclesUsed; i++) {
			ppu.EmulateCycle();
		}
	}
}