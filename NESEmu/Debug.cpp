#include "Debug.h"

void Debug::LogCPU(int PC, int OC, int A, int X, int Y, int P, int S, int C) {
	printf("PC: %X, O: %X, A: %X, X: %X, Y: %X, P:%X, S: %X, CYC: %X\n", PC, OC, A, X, Y, P, S, C);
}