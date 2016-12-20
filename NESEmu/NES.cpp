#include "NES.h"

void NES::Initialize() {
	cpu.Initialize(&memory);
	cpu.Reset();
	ppu.Initialize(&memory, &cpu.NMI);
}

void NES::Run() {
	unsigned short cyclesUsed;
	for (;;) {
		cyclesUsed = cpu.EmulateCycle();

		for (int i = 0; i < cyclesUsed * 3; i++) {
			ppu.EmulateCycle();
		}
	}
}