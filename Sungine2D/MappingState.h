#pragma once

#include "FSM.h"

#include "SuTexture2D.h"
#include "ECSEntityManager.h"
#include "ECSHandler.h"

class MappingState : public State
{
private:
	SuTexture2D texture;
	std::vector<unsigned int> row;
	std::vector<std::vector<unsigned int>> tileData;
	ECSEntity entityData[12][20];
	unsigned int currentTile = 1;

	void InitLevel(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
	void LoadLevel(const char* file);

public:
	MappingState() : State("MappingState") {}
	void Enter();
	void Update(float deltaTime);
	void Render();
	void Resume() {}
	void Exit();
};

