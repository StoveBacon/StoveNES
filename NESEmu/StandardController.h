#pragma once
#include "SDLWrapper.h"
#include "Memory.h"

class StandardController {
	Memory *memory;
	SDLWrapper *wrapper;
public:
	void Initialize();
	void PollInput();
};