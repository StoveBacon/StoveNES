#pragma once
#include "PPURegisters.h"
#include <string>
#include <fstream>

/* Memory map
* 0x0000 - 0x00FF Zero Page
* 0x0100 - 0x01FF Stack
* 0x0200 - 0x07FF Work RAM
* 0x0800 - 0x1FFF Mirror of Work RAM
* 0x2000 - 0x2007 PPU Registers
* 0x2008 - 0x3FFF Mirrors of PPUR
* 0x4000 - 0x4017 APU and I/O Registers
* 0x4018 - 0x401F Disabled APU and I/O
* 0x4020 - 0xFFFF PRG ROM, PRG RAM, Mapper Registers
*/

class Memory {
	// CPU and PPU memory
	unsigned char memory[0xFFFF] = { 0 };

	// PPU Video RAM
	unsigned short VRAM[0x3FFF] = { 0 };

	// Object attribute memory
	unsigned short OAMAddr = 0;
	unsigned short OAM[0x100] = { 0 };

	// Latch for reading and writing between PPU and CPU
	unsigned short addressLatch = 0;

public:
	unsigned short ReadByte(unsigned short index);
	unsigned short ReadBytes(unsigned short index, unsigned int numBytes);
	unsigned short ReadByteVRAM(unsigned short index);
	unsigned short ReadByteOAM(unsigned short index);
	void WriteByte(unsigned short index, unsigned short byte);

	// PPU Functions
	void WriteToVRAMAddress(unsigned short byte);
	void WriteToVRAM(unsigned short byte);
	void WriteToOAM(unsigned short byte);
	void IncrementAddressLatch();
	void LoadOAM(unsigned short byte);

	void LoadCartridge(std::string path);
};