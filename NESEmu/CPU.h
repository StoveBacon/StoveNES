#pragma once
#include "StatusRegister.h"
#include "Memory.h"
#include <stdio.h>

// The 6502 CPU core at the heart of the NES
class CPU {
	/* CPU registers
	* Accumulator A
	* Indexes X and Y
	* Program Counter PC
	* Stack Pointer S
	* Status Register P
	*/
	unsigned short A;
	unsigned short X, Y;
	unsigned short PC;
	unsigned short S;
	StatusRegister P;

	unsigned short stack[0xFF] = { 0 };

	// Opcode retainer
	unsigned char opcode;

	// Cycle Counter
	unsigned int cycles;

	// Memory
	Memory memory;

public:
	void Initialize();
	void EmulateCycle();
	unsigned short ADC(unsigned short a, unsigned short b);
	unsigned short SBC(unsigned short a, unsigned short b);
	void CMP(unsigned short a, unsigned short b);
};