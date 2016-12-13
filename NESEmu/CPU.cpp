#include "CPU.h"

void CPU::Initialize() {
	// Zero everything out for now
	PC = A = X = Y = cycles = 0;
	P.SetAll(0b00110000);

	// The stack ranges from 0x1FF to 0x100 (descending stack)
	// The stack pointer is also offset by 0x100 in stack operations
	S = 0x1FF;

	// DEBUG MEMORY PLS REMOVE
	memory.WriteByte(0x00, 0xA9);
	memory.WriteByte(0x01, 0x01);
	memory.WriteByte(0x02, 0x69);
	memory.WriteByte(0x03, 0x01);
}

void CPU::EmulateCycle() {

	// Debug log the current state of the processor before the next instruction
	printf("Before Next Cycle:\nPC: %X, A: %X, X: %X, Y: %X, S: %X, Cycles: %X, Flags: %X\n\n", PC, A, X, Y, S, cycles, P.ToByte());

	// Fetch the next byte of information and advance the program counter
	opcode = memory.ReadByte(PC);
	PC += 1;

	// The operand to be loaded into after decoding the opcode
	unsigned short operand;

	// Used to store temporary information
	unsigned short value;
	unsigned short carry;

	/* Opcode decoding
	*  Ordered by functionality, including all 
	*  addressing modes for each operation
	*/
	switch (opcode) {

		// --- Memory operations ---
#pragma region Memory_Operations
		// LDA - Load Accumulator
	case (0xA9): // Immediate
		operand = memory.ReadByte(PC);
		A = operand;
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 2;
		break;

	case (0xA5): // Zero Page
		operand = memory.ReadByte(PC);
		A = memory.ReadByte(operand);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 3;
		break;

	case (0xB5): // Zero Page, X
		operand = memory.ReadByte(PC);
		A = memory.ReadByte(operand + X);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 4;
		break;

	case (0xAD): // Absolute
		operand = memory.ReadBytes(PC, 2);
		A = memory.ReadByte(operand);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0xBD): // Absolute, X
		operand = memory.ReadBytes(PC, 2);
		A = memory.ReadByte(operand + X);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0xB9): // Absolute, Y
		operand = memory.ReadBytes(PC, 2);
		A = memory.ReadByte(operand + Y);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0xA1): // Indirect, X
		operand = memory.ReadByte(PC);
		A = memory.ReadByte(memory.ReadByte(operand + X));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 6;
		break;

	case (0xB1): // Indirect, Y
		operand = memory.ReadByte(PC);
		A = memory.ReadByte(memory.ReadByte(operand) + Y);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 5;
		break;

		// LDX - Load X
	case (0xA2): // Immediate
		operand = memory.ReadByte(PC);
		X = operand;
		P.DetermineNegative(X);
		P.DetermineZero(X);
		PC++;
		cycles += 2;
		break;

	case (0xA6): // Zero Page
		operand = memory.ReadByte(PC);
		X = memory.ReadByte(operand);
		P.DetermineNegative(X);
		P.DetermineZero(X);
		PC++;
		cycles += 3;
		break;

	case (0xB6): // Zero Page, Y
		operand = memory.ReadByte(PC);
		X = memory.ReadByte(operand + Y);
		P.DetermineNegative(X);
		P.DetermineZero(X);
		PC++;
		cycles += 4;
		break;

	case (0xAE): // Absolute
		operand = memory.ReadBytes(PC, 2);
		X = operand;
		P.DetermineNegative(X);
		P.DetermineZero(X);
		PC += 2;
		cycles += 4;
		break;

	case (0xBE): // Absolute, Y
		operand = memory.ReadBytes(PC, 2);
		X = memory.ReadByte(operand + Y);
		P.DetermineNegative(X);
		P.DetermineZero(X);
		PC += 2;
		cycles += 4;
		break;

		// LDY - Load Y
	case (0xA0): // Immediate
		operand = memory.ReadByte(PC);
		Y = operand;
		P.DetermineNegative(Y);
		P.DetermineZero(Y);
		PC++;
		cycles += 2;
		break;

	case (0xA4): // Zero Page
		operand = memory.ReadByte(PC);
		Y = memory.ReadByte(operand);
		P.DetermineNegative(Y);
		P.DetermineZero(Y);
		PC++;
		cycles += 3;
		break;

	case (0xB4): // Zero Page, X
		operand = memory.ReadByte(PC);
		Y = memory.ReadByte(operand + X);
		P.DetermineNegative(Y);
		P.DetermineZero(Y);
		PC++;
		cycles += 4;
		break;

	case (0xAC): // Absolute
		operand = memory.ReadBytes(PC, 2);
		Y = memory.ReadByte(operand);
		P.DetermineNegative(Y);
		P.DetermineZero(Y);
		PC += 2;
		cycles += 4;
		break;

	case (0xBC): // Absolute, X
		operand = memory.ReadBytes(PC, 2);
		Y = memory.ReadByte(operand + X);
		P.DetermineNegative(Y);
		P.DetermineZero(Y);
		PC += 2;
		cycles += 4;
		break;

		// STA - Store Accumulator
	case (0x85): // Zero Page
		operand = memory.ReadByte(PC);
		memory.WriteByte(operand, A);
		PC++;
		cycles += 3;
		break;

	case (0x95): // Zero Page, X
		operand = memory.ReadByte(PC);
		memory.WriteByte(operand + X, A);
		PC++;
		cycles += 4;
		break;

	case (0x8D): // Absolute
		operand = memory.ReadBytes(PC, 2);
		memory.WriteByte(operand, A);
		PC += 2;
		cycles += 4;
		break;

	case (0x9D): // Absolute, X
		operand = memory.ReadBytes(PC, 2);
		memory.WriteByte(operand + X, A);
		PC += 2;
		cycles += 4;
		break;

	case (0x99): // Absolute, Y
		operand = memory.ReadBytes(PC, 2);
		memory.WriteByte(operand + Y, A);
		PC += 2;
		cycles += 4;
		break;

	case (0x81): // Indirect, X
		operand = memory.ReadByte(PC);
		memory.WriteByte(memory.ReadByte(memory.ReadByte(operand + X)), A);
		PC++;
		cycles += 6;
		break;

	case (0x91): // Indirect, Y
		operand = memory.ReadByte(PC);
		memory.WriteByte(memory.ReadByte(memory.ReadByte(operand) + Y), A);
		PC++;
		cycles += 5;
		break;

		// STX - Store X
	case (0x86): // Zero Page
		operand = memory.ReadByte(PC);
		memory.WriteByte(operand, X);
		PC++;
		cycles += 3;
		break;

	case (0x96): // Zero Page, Y
		operand = memory.ReadByte(PC);
		memory.WriteByte(operand + Y, X);
		PC++;
		cycles += 4;
		break;

	case (0x8E): // Absolute
		operand = memory.ReadBytes(PC, 2);
		memory.WriteByte(operand, X);
		PC += 2;
		cycles += 4;
		break;

		// STY - Store Y
	case (0x84): // Zero Page
		operand = memory.ReadByte(PC);
		memory.WriteByte(operand, Y);
		PC++;
		cycles += 3;
		break;

	case (0x94): // Zero Page, X
		operand = memory.ReadByte(PC);
		memory.WriteByte(operand + X, Y);
		PC++;
		cycles += 4;
		break;

	case (0x8C): // Absolute
		operand = memory.ReadBytes(PC, 2);
		memory.WriteByte(operand, Y);
		PC += 2;
		cycles += 4;
		break;
#pragma endregion Memory_Operations

		// --- Register Transfer Operations ---
#pragma region Register_Transfer_Operations
	case (0xAA): // Transfer A to X
		X = A;
		P.DetermineNegative(X);
		P.DetermineZero(X);
		PC++;
		cycles += 2;
		break;

	case (0xA8): // Transfer A to Y
		Y = A;
		P.DetermineNegative(Y);
		P.DetermineZero(Y);
		PC++;
		cycles += 2;
		break;

	case (0x8A): // Transfer X to A
		A = X;
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 2;
		break;

	case (0x98): // Transfer Y to A
		A = Y;
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 2;
		break;

	case (0xBA): // Transfer SP to X
		X = S;
		P.DetermineNegative(X);
		P.DetermineZero(X);
		PC++;
		cycles += 2;
		break;

	case (0x9A): // Transfer X to SP
		S = X;
		P.DetermineNegative(X);
		P.DetermineZero(X);
		PC++;
		cycles += 2;
		break;
#pragma endregion Register_Transfer_Operations

		// --- Logical Operations ---
#pragma region Logical_Operations
		// AND - Logical and ( with accumulator)
	case (0x29): // Immediate
		operand = memory.ReadByte(PC);
		A = A & operand;
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 2;
		break;

	case (0x25): // Zero Page
		operand = memory.ReadByte(PC);
		A = A & memory.ReadByte(operand);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 3;
		break;

	case (0x35): // Zero Page, X
		operand = memory.ReadByte(PC);
		A = A & memory.ReadByte(operand + X);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 4;
		break;

	case (0x2D): // Absolute
		operand = memory.ReadBytes(PC, 2);
		A = A & memory.ReadByte(operand);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x3D): // Absolute, X
		operand = memory.ReadBytes(PC, 2);
		A = A & memory.ReadByte(operand + X);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x39): // Absolute, Y
		operand = memory.ReadBytes(PC, 2);
		A = A & memory.ReadByte(operand + Y);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x21): // Indirect, X
		operand = memory.ReadByte(PC);
		A = A & memory.ReadByte(memory.ReadByte(operand + X));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 6;
		break;

	case (0x31): // Indirect, Y
		operand = memory.ReadByte(PC);
		A = A & memory.ReadByte(memory.ReadByte(operand) + Y);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 6;
		break;

		// EOR - Exclusive or (with accumulator)
	case (0x49): // Immediate
		operand = memory.ReadByte(PC);
		A = A ^ operand;
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 2;
		break;

	case (0x45): // Zero Page
		operand = memory.ReadByte(PC);
		A = A ^ memory.ReadByte(operand);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 3;
		break;

	case (0x55): // Zero Page, X
		operand = memory.ReadByte(PC);
		A = A ^ memory.ReadByte(operand + X);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 4;
		break;

	case (0x4D): // Absolute
		operand = memory.ReadBytes(PC, 2);
		A = A ^ memory.ReadByte(operand);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x5D): // Absolute, X
		operand = memory.ReadBytes(PC, 2);
		A = A ^ memory.ReadByte(operand + X);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;
		
	case (0x59): // Absolute, Y
		operand = memory.ReadBytes(PC, 2);
		A = A ^ memory.ReadByte(operand + Y);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x41): // Indirect, X
		operand = memory.ReadByte(PC);
		A = A ^ memory.ReadByte(memory.ReadByte(opcode + X));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 6;
		break;

	case (0x51): // Indirect, Y
		operand = memory.ReadByte(PC);
		A = A ^ memory.ReadByte(memory.ReadByte(opcode) + Y);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 5;
		break;

		// ORA - OR with accumulator
	case (0x09): // Immediate
		operand = memory.ReadByte(PC);
		A = A | operand;
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 2;
		break;

	case (0x05): // Zero Page
		operand = memory.ReadByte(PC);
		A = A | memory.ReadByte(operand);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 3;
		break;

	case (0x15): // Zero Page, X
		operand = memory.ReadByte(PC);
		A = A | memory.ReadByte(operand + X);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 4;
		break;

	case (0x0D): // Absolute
		operand = memory.ReadBytes(PC, 2);
		A = A | memory.ReadByte(operand);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x1D): // Absolute, X
		operand = memory.ReadBytes(PC, 2);
		A = A | memory.ReadByte(operand + X);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x19): // Absolute, Y
		operand = memory.ReadBytes(PC, 2);
		A = A | memory.ReadByte(operand + Y);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x01): // Indirect, X
		operand = memory.ReadByte(PC);
		A = A | memory.ReadByte(memory.ReadByte(operand + X));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 6;
		break;

	case (0x11): // Indirect, Y
		operand = memory.ReadByte(PC);
		A = A | memory.ReadByte(memory.ReadByte(operand) + Y);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 5;
		break;
#pragma endregion Logical_Operations

		// --- Arithmetic Operations ---
#pragma region Arithmetic_Operations
		// ADC - Add with carry
	case (0x69): // Immediate
		opcode = memory.ReadByte(PC);
		A = ADC(A, opcode);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 2;
		break;

	case (0x65): // Zero Page
		opcode = memory.ReadByte(PC);
		A = ADC(A, memory.ReadByte(opcode));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 3;
		break;

	case (0x75): // Zero Page, X
		opcode = memory.ReadByte(PC);
		A = ADC(A, memory.ReadByte(opcode + X));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 4;
		break;

	case (0x6D): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		A = ADC(A, memory.ReadByte(opcode));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x7D): // Absolute, X
		opcode = memory.ReadBytes(PC, 2);
		A = ADC(A, memory.ReadByte(opcode + X));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x79): // Absolute, Y
		opcode = memory.ReadBytes(PC, 2);
		A = ADC(A, memory.ReadByte(opcode + Y));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0x61): // Indirect, X
		opcode = memory.ReadByte(PC);
		A = ADC(A, memory.ReadByte(memory.ReadByte(opcode + X)));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 6;
		break;

	case (0x71): // Indirect, Y
		opcode = memory.ReadByte(PC);
		A = ADC(A, memory.ReadByte(memory.ReadByte(opcode) + Y));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 5;
		break;

		// DEC - Decrement
	case (0xC6): // Zero Page
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode) - 1;
		memory.WriteByte(opcode, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC++;
		cycles += 5;
		break;

	case (0xD6): // Zero Page, X
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode + X) - 1;
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC++;
		cycles += 6;
		break;

	case (0xCE): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode) - 1;
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC += 2;
		cycles += 6;
		break;

	case (0xDE): // Absolute, X
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode + X) - 1;
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC += 2;
		cycles += 6;
		break;

		// DEX - Decrement X
	case (0xCA): // Implied
		X = X - 1;
		P.DetermineNegative(X);
		P.DetermineZero(X);
		cycles += 2;
		break;

		// DEY - Decrement Y
	case (0x88):
		Y = Y - 1;
		P.DetermineNegative(Y);
		P.DetermineZero(Y);
		cycles += 2;
		break;

		// INC - Increment
	case (0xE6): // Zero Page
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode) + 1;
		memory.WriteByte(opcode, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC++;
		cycles += 3;
		break;

	case (0xF6): // Zero Page, X
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode + X) + 1;
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC++;
		cycles += 4;
		break;

	case (0xEE): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode) + 1;
		memory.WriteByte(opcode, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC += 2;
		cycles += 4;
		break;

	case (0xFE): // Absolute, X
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode + X) + 1;
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC += 2;
		cycles += 4;
		break;

		// INX - Increment X
	case (0xE8): // Implied
		X = X + 1;
		P.DetermineNegative(X);
		P.DetermineZero(X);
		cycles += 2;
		break;

		// INY - Increment Y
	case (0xC8):
		Y = Y + 1;
		P.DetermineNegative(Y);
		P.DetermineZero(Y);
		cycles += 2;
		break;

		// SBC - Subtract with carry
	case (0xE9): // Immediate
		opcode = memory.ReadByte(PC);
		A = SBC(A, opcode);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 2;
		break;

	case (0xE5): // Zero Page
		opcode = memory.ReadByte(PC);
		A = SBC(A, memory.ReadByte(opcode));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 3;
		break;

	case (0xF5): // Zero Page, X
		opcode = memory.ReadByte(PC);
		A = SBC(A, memory.ReadByte(opcode + X));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 4;
		break;

	case (0xED): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		A = SBC(A, memory.ReadByte(opcode));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0xFD): // Absolute, X
		opcode = memory.ReadBytes(PC, 2);
		A = SBC(A, memory.ReadByte(opcode + X));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0xF9): // Absolute, Y
		opcode = memory.ReadBytes(PC, 2);
		A = SBC(A, memory.ReadByte(opcode + Y));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC += 2;
		cycles += 4;
		break;

	case (0xE1): // Indirect, X
		opcode = memory.ReadByte(PC);
		A = SBC(A, memory.ReadByte(memory.ReadByte(opcode + X)));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 6;
		break;

	case (0xF1): // Indirect, Y
		opcode = memory.ReadByte(PC);
		A = SBC(A, memory.ReadByte(memory.ReadByte(opcode) + Y));
		P.DetermineNegative(A);
		P.DetermineZero(A);
		PC++;
		cycles += 5;
		break;
#pragma endregion Arithmetic_Operations
		
		// --- Bit Manipulation Operations ---
#pragma region Bit_Manipulation_Operations
		// ASL - Arithmetic Shift Left
	case (0x0A): // Accumulator
		P.SetCarry((A & 0x80) >> 7);
		A = A << 1;
		P.DetermineNegative(A);
		P.DetermineZero(A);
		cycles += 2;
		break;

	case (0x06): // Zero Page
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode);
		P.SetCarry((value & 0x80) >> 7);
		value = value << 1;
		memory.WriteByte(opcode, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC++;
		cycles += 5;
		break;

	case (0x16): // Zero Page, X
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode + X);
		P.SetCarry((value & 0x80) >> 7);
		value = value << 1;
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC++;
		cycles += 6;
		break;

	case (0x0E): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode);
		P.SetCarry((value & 0x80) >> 7);
		value = value << 1;
		memory.WriteByte(opcode, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC += 2;
		cycles += 6;
		break;

	case (0x1E): // Absolute, X
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode + X);
		P.SetCarry((value & 0x80) >> 7);
		value = value << 1;
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC += 2;
		cycles += 7;
		break;

		// LSR - Logical Shift Right
	case (0x4A): // Accumulator
		P.SetCarry(A & 0x01);
		A = A >> 1;
		P.DetermineNegative(A);
		P.DetermineZero(A);
		cycles += 2;
		break;

	case (0x46): // Zero Page
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode);
		P.SetCarry(A & 0x01);
		value = value >> 1;
		memory.WriteByte(opcode, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC++;
		cycles += 5;
		break;

	case (0x56): // Zero Page, X
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode + X);
		P.SetCarry(A & 0x01);
		value = value >> 1;
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC++;
		cycles += 6;
		break;

	case (0x4E): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode);
		P.SetCarry(A & 0x01);
		value = value >> 1;
		memory.WriteByte(opcode, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC += 2;
		cycles += 6;
		break;

	case (0x5E): // Absolute, X
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode + X);
		P.SetCarry(A & 0x01);
		value = value >> 1;
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC += 2;
		cycles += 7;
		break;

		// ROL - Rotate Left
	case (0x2A): // Accumulator
		carry = (A & 0x80) >> 7;
		A = A << 1;
		A = A | P.C;
		P.SetCarry(carry);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		cycles += 2;
		break;

	case (0x26): // Zero Page
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode);
		carry = (value & 0x80) >> 7;
		value = value << 1;
		value = value | P.C;
		memory.WriteByte(opcode, value);
		P.SetCarry(carry);
		P.DetermineNegative(value);
		P.SetOverflow(value);
		PC++;
		cycles += 5;
		break;

	case (0x36): // Zero Page, X
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode + X);
		carry = (value & 0x80) >> 7;
		value = value << 1;
		value = value | P.C;
		memory.WriteByte(opcode + X, value);
		P.SetCarry(carry);
		P.DetermineNegative(value);
		P.SetOverflow(value);
		PC++;
		cycles += 6;
		break;

	case (0x2E): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode);
		carry = (value & 0x80) >> 7;
		value = value << 1;
		value = value | P.C;
		memory.WriteByte(opcode, value);
		P.SetCarry(carry);
		P.DetermineNegative(value);
		P.SetOverflow(value);
		PC += 2;
		cycles += 6;
		break;

	case (0x3E): // Absolute, X
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode + X);
		carry = (value & 0x80) >> 7;
		value = value << 1;
		value = value | P.C;
		memory.WriteByte(opcode + X, value);
		P.SetCarry(carry);
		P.DetermineNegative(value);
		P.SetOverflow(value);
		PC += 2;
		cycles += 7;
		break;

		// ROR - Rotate Right
	case (0x6A): // Accumulator
		carry = (A & 0x01);
		A = A >> 1;
		A = A | (P.C << 7);
		P.SetCarry(carry);
		P.DetermineNegative(A);
		P.DetermineZero(A);
		cycles += 2;
		break;

	case (0x66): // Zero Page
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode);
		carry = (value & 0x01);
		value = value >> 1;
		value = value | (P.C << 7);
		memory.WriteByte(opcode, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC++;
		cycles += 5;
		break;

	case (0x76): // Zero Page, X
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode + X);
		carry = (value & 0x01);
		value = value >> 1;
		value = value | (P.C << 7);
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC++;
		cycles += 6;
		break;

	case (0x6E): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode);
		carry = (value & 0x01);
		value = value >> 1;
		value = value | (P.C << 7);
		memory.WriteByte(opcode, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC += 2;
		cycles += 6;
		break;

	case (0x7E): // Absolute, X
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode + X);
		carry = (value & 0x01);
		value = value >> 1;
		value = value | (P.C << 7);
		memory.WriteByte(opcode + X, value);
		P.DetermineNegative(value);
		P.DetermineZero(value);
		PC += 2;
		cycles += 6;
		break;
#pragma endregion Bit_Manipulation_Operations

		// --- Stack Operations ---
#pragma region Stack_Operations
		// PHA - Push Accumulator
	case (0x48):
		stack[S] = A;
		S--;
		cycles += 3;
		break;

		// PHP - Push Processor Status
	case (0x08):
		stack[S] = P.ToByte();
		S--;
		cycles += 3;
		break;

		// PLA - Pull Accumulator
	case (0x68):
		S++;
		A = stack[S];
		P.DetermineNegative(A);
		P.DetermineZero(A);
		cycles += 4;
		break;

		// PLP - Pull Processor Status
	case (0x28):
		S++;
		P.SetAll(stack[S]);
		cycles += 4;
		break;
#pragma endregion Stack_Operations

		// --- Subroutine Operations ---
#pragma region Subroutine_Operations
		// JMP - Jump
	case (0x4C): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		PC = opcode;
		cycles += 3;
		break;

	case (0x6C): // Indirect
		opcode = memory.ReadBytes(PC, 2);
		PC = memory.ReadByte(opcode);
		cycles += 5;
		break;

		// JSR - Jump to Subroutine
	case (0x20): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		stack[S] = P.ToByte();
		stack[S - 1] = PC;
		S -= 2;
		PC = opcode;
		cycles += 6;
		break;

		// RTI - Return from interupt
	case (0x40): // Implied
		// TBI
		break;

		// RTS - Return from Subroutine
	case (0x60): // Implied
		S += 2;
		P.SetAll(stack[S]);
		PC = stack[S - 1];
		PC++;
		cycles += 6;
		break;
#pragma endregion Subroutine_Operations

		// --- Comparison Operations ---
#pragma region Comparison_Operations
		// BIT - Logical &s the acumulator with a memory value (Google for more information)
	case (0x24): // Zero Page
		opcode = memory.ReadByte(PC);
		value = A & memory.ReadByte(opcode);
		P.DetermineZero(value);
		P.DetermineNegative(value);
		P.SetOverflow((value & 0x40) >> 6);
		PC++;
		cycles += 3;
		break;

	case (0x2C): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		value = A & memory.ReadByte(opcode);
		P.DetermineZero(value);
		P.DetermineNegative(value);
		P.SetOverflow((value & 0x40) >> 6);
		PC += 2;
		cycles += 4;
		break;

		// CMP - Compare with Accumulator
	case (0xC9): // Immediate
		opcode = memory.ReadByte(PC);
		CMP(A, opcode);
		PC++;
		cycles += 2;
		break;

	case (0xC5): // Zero Page
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode);
		CMP(A, value);
		PC++;
		cycles += 3;
		break;

	case (0xD5): // Zero Page, X
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode);
		CMP(A, value);
		PC += 2;
		cycles += 4;
		break;

	case (0xCD): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode);
		CMP(A, value);
		PC += 2;
		cycles += 4;
		break;

	case (0xDD): // Absolute, X
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode + X);
		CMP(A, value);
		PC += 2;
		cycles += 4;
		break;

	case (0xD9): // Absolute, Y
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode + Y);
		CMP(A, value);
		PC += 2;
		cycles += 4;
		break;

	case (0xC1): // Indirect, X
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(memory.ReadByte(opcode + X));
		CMP(A, value);
		PC++;
		cycles += 6;
		break;

	case (0xD1): // Indirect, X
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(memory.ReadByte(opcode) + Y);
		CMP(A, value);
		PC++;
		cycles += 5;
		break;

		// CPX - Compare X
	case (0xE0): // Immediate
		opcode = memory.ReadByte(PC);
		CMP(X, opcode);
		PC++;
		cycles += 2;
		break;

	case (0xE4): // Zero Page
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode);
		CMP(X, value);
		PC++;
		cycles += 3;
		break;

	case (0xEC): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode);
		CMP(X, value);
		PC += 3;
		cycles += 4;
		break;

		// CPX - Compare Y
	case (0xC0): // Immediate
		opcode = memory.ReadByte(PC);
		CMP(Y, opcode);
		PC++;
		cycles += 2;
		break;

	case (0xC4): // Zero Page
		opcode = memory.ReadByte(PC);
		value = memory.ReadByte(opcode);
		CMP(Y, value);
		PC++;
		cycles += 3;
		break;

	case (0xCC): // Absolute
		opcode = memory.ReadBytes(PC, 2);
		value = memory.ReadByte(opcode);
		CMP(Y, value);
		PC += 3;
		cycles += 4;
		break;
#pragma endregion Comparison_Operations

		// --- Status Operations ---
#pragma region Status_Operations
		// BRK - Break (interrupt)
	case (0x00):
		// TBI
		break;

		// CLC - Clear Carry
	case (0x18):
		P.SetCarry(0);
		cycles += 2;
		break;

		// SEC - Set Carry
	case (0x38):
		P.SetCarry(1);
		cycles += 2;
		break;

		// CLI - Clear Interrupt
	case (0x58):
		P.SetInterrupt(0);
		cycles += 2;
		break;

		// SEI - Set Interrupt
	case (0x78):
		P.SetInterrupt(1);
		cycles += 2;
		break;

		// CLV - Clear Overflow
	case (0xB8):
		P.SetOverflow(0);
		cycles += 2;
		break;

		// CLD - Clear Decimal
	case (0xD8):
		// Not supported
		cycles += 2;
		break;

		// SED - Set Decimal
	case (0xF8):
		// Not supported
		cycles += 2;
		break;

		// NOP - No Operation
	case (0xEA):
		cycles += 2;
		break;
#pragma endregion Status_Operations

		// -- Branching Operations ---
#pragma region Branching_Operations
		//BPL - Branch on Plus
	case (0x10):
		opcode = memory.ReadByte(PC);
		PC += P.N ? 1 : opcode;
		cycles += P.N ? 2 : 3;
		break;

		//BMI - Branch on Minues
	case (0x30):
		opcode = memory.ReadByte(PC);
		PC += P.N ? opcode : 1;
		cycles += P.N ? 3 : 2;
		break;

		// BVC - Branch on Overflow Clear
	case (0x50):
		opcode = memory.ReadByte(PC);
		PC += P.V ? 1 : opcode;
		cycles += P.V ? 2 : 3;
		break;

		//BMI - Branch on Overflow Set
	case (0x70):
		opcode = memory.ReadByte(PC);
		PC += P.V ? opcode : 1;
		cycles += P.V ? 3 : 2;
		break;

		// BCC - Branch on Carry Clear
	case (0x90):
		opcode = memory.ReadByte(PC);
		PC += P.C ? 1 : opcode;
		cycles += P.C ? 2 : 3;
		break;

		// BCS - Branch on Carry Set
	case (0xB0):
		opcode = memory.ReadByte(PC);
		PC += P.C ? opcode : 1;
		cycles += P.C ? 3 : 2;
		break;

		// BNE - Branch on Not Equal (Zero Flag Clear)
	case (0xD0):
		opcode = memory.ReadByte(PC);
		PC += P.C ? 1 : opcode;
		cycles += P.C ? 2 : 3;
		break;

		// BEQ - Branch on Equal (Zero Flag Set)
	case (0xF0):
		opcode = memory.ReadByte(PC);
		PC += P.C ? opcode : 1;
		cycles += P.C ? 3 : 2;
		break;
#pragma endregion Branching_Operations

	default:
		printf("Unknown opcode 0x%X", opcode);
	}
}

// Adds with carry, setting the appropriate flags
unsigned short CPU::ADC(unsigned short a, unsigned short b) {
	unsigned int result = a + b;

	// If a and b have the same sign, and a and the result have a different sign,
	// that means that there was an overflow
	unsigned short overflow = (((~(a ^ b)) & (a ^ result)) & 0x80) >> 7;
	P.SetOverflow(overflow);

	// If there is a carry
	P.SetCarry(result > 0xFF);

	result = result % 0x100;

	return result;
}

// Subtract with carry simply inverts the arguments and passes them to ADC
unsigned short CPU::SBC(unsigned short a, unsigned short b) {
	return ADC(~a, ~b);
}

// Sets the flags based off the provided values
void CPU::CMP(unsigned short a, unsigned short b) {
	P.SetCarry(a >= b);
	P.SetZero(a == b);
	P.SetNegative(a < b);
}