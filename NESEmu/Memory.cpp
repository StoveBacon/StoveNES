#include "Memory.h"

Memory* Memory::instance = NULL;

Memory* Memory::Instance() {
	if (instance == NULL) {
		instance = new Memory();
	}

	return instance;
}

// Directly returns a byte from memory
unsigned short Memory::ReadByte(unsigned short index) {

	// Handle mirroring
	if (index >= 0x1000 && index <= 0x1FFF) {
		index = index % 0x0800;
	}
	if (index >= 0x2008 && index <= 0x3FFF) {
		index = index % 8;
		index += 0x2000;
	}

	// Check for register reads
	switch (index) {

		case (PPUSTATUS):
			addressLatch = 0;
			break;

		case (JOYPAD1):
		{
			unsigned short temp = memory[JOYPAD1];
			memory[JOYPAD1] = temp >> 1;
			return temp & 1;
			// Temp to get nestest working
			if ((i % 8) == 3) {
				i++;
				return 1;
			}
			else
			{
				i++;
				return 0;
			}
		}
		break;

		case (PPUDATA):
		{
			unsigned short tempAddr = addressLatch;
			unsigned short tempBuffer = vramBuffer; // For whatever reason the PPU holds a buffer to the last vram data and THEN updates it
			vramBuffer = VRAM[tempAddr];
			IncrementAddressLatch();
			return tempBuffer;
		}
		break;
	}

	return memory[index];
}

unsigned short Memory::ReadByteVRAM(unsigned short index) {
	return VRAM[index];
}

// Given an index and a number of bytes to read, returns the bytes in big-endian form,
// as the 6502 is a little-endian system
unsigned short Memory::ReadBytes(unsigned short index, unsigned int numBytes) {
	unsigned short mem = 0;
	for (int i = numBytes - 1; i >= 0; i--) {
		// Provide Zero Page wraparound
		if (index <= 0xFF) {
			mem = mem | (ReadByte((index + i) % 0x100) << (i * 8));
		} else {
			mem = mem | (ReadByte(index + i) << (i * 8));
		}
	}

	return mem;
}

// Writes to memory, calls specific functions on special cases (VRAM, OAM)
void Memory::WriteByte(unsigned short index, unsigned short byte) {
	
	// Check for register writes
	switch (index) {

	case (OAMADDR): // OAM Address
		OAMAddr = byte;
		break;

	case (OAMDATA): // OAM Write
		WriteToOAM(byte);
		break;

	case (PPUADDR): // VRAM Address write
		WriteToVRAMAddress(byte);
		break;

	case (PPUDATA): // VRAM Write
		WriteToVRAM(byte);
		IncrementAddressLatch();
		break;

	case (OAMDMA): // Direct access OAM
		LoadOAM(byte);
		break;

	case (JOYPAD1): // Controller Write
		if (byte == 0) {
			controllerStrobe = false;
		}
		else if (byte & 0x01)
		{
			controllerStrobe = true;
		}
		break;

	default:
		if (byte & 0xFF00) {
			memory[index] = (byte & 0xFF00) >> 8;
			memory[index + 1] = byte & 0xFF;
		} else
			memory[index] = byte;
		break;
	}
}

void Memory::WriteControllerData(unsigned short data) {
	memory[JOYPAD1] = data;
}

void Memory::WriteToVRAMAddress(unsigned short byte) {
	if (addressLatch == 0) {
		addressLatch += byte;
	} else {
		addressLatch = addressLatch << 8;
		addressLatch += byte;
		addressLatch &= 0xFFFF;
	}
}

void Memory::WriteToVRAM(unsigned short byte) {
	VRAM[addressLatch] = byte;

	// OPTIMIZATION: We need to watch for palette swaps so the PPU doesn't have to check for every single pixel
	if (addressLatch >= PALETTE && addressLatch <= PALETTE_END) {
		paletteHasChanged = true;
	}
}

// If there has been an update clear the flag and return true
bool Memory::PaletteHasChanged() {
	if (paletteHasChanged) {
		paletteHasChanged = false;
		return true;
	}

	return false;
}

void Memory::IncrementAddressLatch() {
	unsigned short incMode = ReadByte(PPUCTRL);
	incMode = incMode & PPUCTRL::INCREMENT_MODE;
	addressLatch += incMode ? 32 : 1;
}

void Memory::WriteToOAM(unsigned short byte) {
	OAM[OAMAddr] = byte;
	OAMAddr++;
}

unsigned short Memory::ReadByteOAM(unsigned short index) {
	return OAM[index];
}

// Called on write to 0x4014
void Memory::LoadOAM(unsigned short byte) {
	for (int i = 0 + OAMAddr; i < 0x100; i++) {
		OAM[i] = ReadByte((byte << 8) + i);
	}
}

// NROM loader, for testing right now
void Memory::LoadCartridge(std::string path) {
	std::ifstream file;
	file.open(path, std::ios::binary);

	if (file.is_open()) {
		// Header starts with NES\n, so skip that
		file.get();
		file.get();
		file.get();
		file.get();

		// Load the header information
		unsigned int PRGRsize = file.get(); // in 16kb units
		unsigned int CHRRsize = file.get(); // in 8kb units
		unsigned int flags6 = file.get();
		unsigned int flags7 = file.get();
		unsigned int PRGRsize8 = file.get(); // in 8kb units
		unsigned int flags9 = file.get();
		unsigned int flags10 = file.get();
		// Unused values 11 - 15
		file.get();
		file.get();
		file.get();
		file.get();
		file.get();

		// if 16KB, mirror memory
		if (PRGRsize == 1) {
			for (int i = 0; i <= 0x3FFF; i++) {
				unsigned int value = file.get();
				WriteByte(0x8000 + i, value);
				WriteByte(0xC000 + i, value);
			}
		} else {
			for (int i = 0; i <= 0x7FFF; i++) {
				unsigned int value = file.get();
				WriteByte(0x8000 + i, value);
			}
		}
		for (int i = 0; i < 0x1FFF; i++) {
			VRAM[i] = file.get();
		}
		file.close();
	}
}