#include "Memory.h"

// Directly returns a byte from memory
unsigned char Memory::ReadByte(unsigned short index) {
	return memory[index];
}

// Given an index and a number of bytes to read, returns the bytes in big-endian form,
// as the 6502 is a little-endian system
unsigned char Memory::ReadBytes(unsigned short index, unsigned int numBytes) {
	unsigned char mem = 0;
	for (int i = numBytes - 1; i >= 0; i--) {
		mem = mem | (memory[index + i] << (i * 8));
	}

	return mem;
}

void Memory::WriteByte(unsigned short index, unsigned short byte) {
	memory[index] = byte;
}