#pragma once
#include "StatusRegister.h"
#include "Memory.h"
#include "FileManager.h"
#include "Debug.h"
#include <iostream>
#include <stdio.h>
#include <fstream>

// The 6502 CPU core at the heart of the NES
class CPU {
	/* CPU registers
	* Accumulator A
	* Indexes X and Y
	* Program Counter PC (keep track of last PC for debug logging)
	* Stack Pointer S
	* Status Register P
	*/
	unsigned short A;
	unsigned short X, Y;
	unsigned short PC, lastPC;
	unsigned short S;
	StatusRegister P;

	const int stackOffset = 0x100;

	// Opcode retainer
	unsigned short opcode;

	// For performing data operations
	unsigned short operand;

	// Cycle Counter
	unsigned int cycles;

	// Memory
	Memory* memory;

	FileManager* fm;

public:
	void Initialize();
	void Reset();
	short EmulateCycle(); // Returns the number of cycles the CPU used

	// Checks for a NMI and if there is one, pushes processor state and jumps to handler
	bool NMI;
	bool IRQ;
	void CheckNMI();
	void CheckIRQ();

	unsigned short ADC(unsigned short a, unsigned short b);
	unsigned short SBC(unsigned short a, unsigned short b);
	void CMP(unsigned short a, unsigned short b);
	unsigned short IndX(unsigned short operand);
	unsigned short JmpInd();
	short GetRelative(unsigned short operand);

	// Returns an array of all registers
	unsigned short *Status();
};