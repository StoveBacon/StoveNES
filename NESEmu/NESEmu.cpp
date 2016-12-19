#include "NES.h"
#include "SDLWrapper.h"

int main(int argc, char* argv[])
{
	NES nes;

	nes.Initialize();
	nes.Run();

    return 0;
}

