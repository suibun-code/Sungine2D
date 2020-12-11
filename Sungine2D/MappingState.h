#pragma once

#include "FSM.h"

#include "SuTexture2D.h"
#include "ECSEntityManager.h"
#include "ECSHandler.h"

class MappingState : public State
{
private:
	//IMGUI
	bool mDisplayMapper = true;
	bool mCollapsed = false;

	bool mMMBState = false;

	std::vector<std::string> mTextures;

	SuTexture2D texture;
	std::vector<unsigned int> row;
	std::vector<std::vector<unsigned int>> tileData;
	ECSEntity entityData[12][20];
	unsigned int currentTile = 0;

	void InitLevel(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight);
	void LoadLevel(const char* file);
	void UpdateImGui();

public:
	MappingState() : State("MappingState") {}
	void Enter();
	void HandleStateEvents(SDL_Event* event);
	void Update(float deltaTime);
	void Render();
	void LateUpdate(float deltaTime);
	void Resume() {}
	void Exit();
};

