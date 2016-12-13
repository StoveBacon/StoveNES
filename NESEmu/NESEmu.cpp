#include "CPU.h"
#include <iostream>

int main()
{
	CPU cpu;

	cpu.Initialize();

	cpu.EmulateCycle();
	cpu.EmulateCycle();
	cpu.EmulateCycle();

	system("pause");

    return 0;
}

