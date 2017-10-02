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
	file.open("config.cfg");

	if (file.is_open()) {
		file >> romPath;
		file >> upscaleAmount;
		file.close();
	}
}