#pragma once

// Contains all the flags for the status register
class StatusRegister {
public:
	// Carry
	bool C;

	// Zero
	bool Z;
	
	// Interrupt Disable
	bool I;

	// Decimal Mode
	bool D;

	// Break Command
	bool B;

	// Overflow Flag
	bool V;

	// Negative Flag
	bool N;

	void SetCarry(unsigned short carry);
	void SetZero(unsigned short zero);
	void SetInterrupt(unsigned short interrupt);
	void SetDecimal(unsigned short decimal);
	void SetOverflow(unsigned short overflow);
	void SetNegative(unsigned short negative);

	void DetermineZero(unsigned short result);
	void DetermineNegative(unsigned short result);

	unsigned short ToByte();
	void SetAll(unsigned short flags);
};