#pragma once

class Memory {
	unsigned char memory[0xFFFF] = { 0 };

public:
	unsigned char ReadByte(unsigned short index);
	unsigned char ReadBytes(unsigned short index, unsigned int numBytes);
	void WriteByte(unsigned short index, unsigned short byte);
};