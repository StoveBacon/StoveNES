#pragma once
#include <stdio.h>
#include <fstream>
#include <string>

class FileManager {
	static FileManager *instance;

public:
	std::string romPath;
	int upscaleAmount;

	static FileManager* Instance();
	void Initialize();
};