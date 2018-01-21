#include "FileManager.h"

FileManager* FileManager::instance = NULL;

FileManager* FileManager::Instance() {
	if (instance == NULL) {
		instance = new FileManager();
		instance->Initialize();
	}

	return instance;
}

void FileManager::Initialize() {
	std::ifstream file;
	file.open("config.txt");

	if (file.is_open()) {
		file >> romPath;
		file >> upscaleAmount;
		file.close();
	}

	// Extract game name from filepath
	gameName = romPath;
	// Erase the .nes
	gameName.erase(gameName.end() - 4, gameName.end());
	// Erase the folders leading up to the game name
	gameName.erase(gameName.begin(), gameName.begin() + static_cast<int>(gameName.find_last_of("/")) + 1);
}

void FileManager::DebugLog(std::string string) {
	if (debug == NULL) {
		debug = new std::ofstream;
		std::stringstream debugPath;
		time_t currentTime = time(0);
		tm *ltm = localtime(&currentTime);

		debugPath << "./DebugLogs/" << gameName << std::setfill('0') << std::setw(2) << ltm->tm_mon + 1 << std::setw(2) << ltm->tm_mday << ltm->tm_year + 1900 
			<< "_" << std::setw(2) << ltm->tm_sec << std::setw(2) << ltm->tm_min << std::setw(2) << ltm->tm_hour << ".txt";
		debug->open(debugPath.str());

		LoadDebugMap();
	}

	*debug << string;
}

// Opens the debug definitions file and puts it all in a map for use by the debugger for more verbose debugging
void FileManager::LoadDebugMap() {
	if (!debugMap.empty()) {
		return;
	}

	std::ifstream file;
	file.open("DebugDefinitions.txt");

	if (!file.is_open()) {
		DebugLog("Unable to open debug definitions");
		return;
	}

	std::string index;
	std::string arg1, arg2;
	std::stringstream hexArg;
	file >> arg1;

	while (!file.eof()) {
		hexArg << std::setfill('0') << std::setw(2) << arg1;
		index = hexArg.str();
		file >> arg2;
		debugMap[index] = arg2;

		file >> arg1;
		if (arg1 == "=") {
			debugMap[index].append(" = ");
			file >> arg1;
		}
		hexArg.str(std::string());
	}

	file.close();
}

std::map<std::string, std::string> *FileManager::GetDebugMap() {
	LoadDebugMap();
	return &debugMap;
}


FileManager::~FileManager() {
	delete instance;
	delete debug;
}