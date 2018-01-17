#include "StandardController.h"

void StandardController::Initialize() {
	this->memory = Memory::Instance();
	this->wrapper = SDLWrapper::Instance();
}

void StandardController::PollInput() {
	unsigned short keys = wrapper->GetKeys();
	memory->WriteByte(0x4016, keys);
}