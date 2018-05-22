#include "StandardController.h"

void StandardController::Initialize() {
	this->memory = Memory::Instance();
	this->wrapper = SDLWrapper::Instance();
}

void StandardController::PollInput() {
	if (memory->controllerStrobe) {
		memory->WriteControllerData(wrapper->GetKeys());
	}
}

unsigned int StandardController::getNextBit() {
	unsigned int ret = keys & 0x01;
	keys >> 1;
	return ret;
}