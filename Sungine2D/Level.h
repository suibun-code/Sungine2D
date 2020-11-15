#pragma once


#include <vector>

class Level
{
private:
	unsigned int tileWidth = 64;
	unsigned int tileHeight = 64;
	unsigned int levelWidth = 0;
	unsigned int levelHeight = 0;

	void Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);

public:
	Level() {}

	void Load(const char* file);
	void Draw();
};