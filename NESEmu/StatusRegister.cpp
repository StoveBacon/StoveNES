#include "StatusRegister.h"

// Directly set the carry flag
void StatusRegister::SetCarry(unsigned short carry) {
	C = carry;
}

// Directly set the zero flag
void StatusRegister::SetZero(unsigned short zero) {
	Z = zero;
}

void StatusRegister::SetInterrupt(unsigned short interrupt) {
	I = interrupt;
}

// Directly set negative flag
void StatusRegister::SetNegative(unsigned short negative) {
	N = negative;
}

// Directly set the overflow flag
void StatusRegister::SetOverflow(unsigned short overflow) {
	V = overflow;
}

// If the result is 0, set the zero flag
void StatusRegister::DetermineZero(unsigned short result) {
	if (result == 0) {
		Z = 1;
	} else {
		Z = 0;
	}
}
// If the most significant bit is 1, then the number is negative
void StatusRegister::DetermineNegative(unsigned short result) {
	N = ((result & 0x80) >> 7);
}

// Convert all the flags into a binary byte
unsigned short StatusRegister::ToByte() {
	unsigned short flags = 0;
	
	flags = flags | (N << 7);
	flags = flags | (V << 6);
	flags = flags | (1 << 5);
	flags = flags | (B << 4);
	flags = flags | (D << 3);
	flags = flags | (I << 2);
	flags = flags | (Z << 1);
	flags = flags | C;

	return flags;
}

// Set all the flags based off of a provided byte
void StatusRegister::SetAll(unsigned short flags) {
	N = (flags & 0x80);
	V = (flags & 0x40);
	B = (flags & 0x10);
	D = (flags & 0x08);
	I = (flags & 0x04);
	Z = (flags & 0x02);
	C = (flags & 0x01);
}