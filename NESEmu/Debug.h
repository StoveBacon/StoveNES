#pragma once
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include "SDL.h"
#include "FileManager.h"
#include "Memory.h"

#define LOG_FRAMERATE true
#define LOG_CPU false

class Debug {
private:
	static std::map<std::string, std::string> *debugMap;

public:
	static void LogCPUStatus(unsigned short *CPUStatus);
	static void PrintFPS(unsigned int *lastFrameTime, unsigned int *cycles);
};