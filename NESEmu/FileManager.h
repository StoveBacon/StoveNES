#pragma once
#include <stdio.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstddef>
#include <map>

class FileManager {
	static FileManager *instance;
	std::string gameName;
	std::ofstream *debug = NULL;
	std::map<std::string, std::string> debugMap;

public:
	~FileManager();

	std::string romPath;
	int upscaleAmount;

	static FileManager* Instance();
	void Initialize();

	void LoadDebugMap();
	void DebugLog(std::string string);
	std::map<std::string, std::string> *GetDebugMap();
};