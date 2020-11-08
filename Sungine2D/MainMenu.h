#pragma once

#include "FSM.h"

#include "ECSEntityManager.h"

class MainMenu :  public State
{
private:
	ECSEntity logo;

public:
	MainMenu() : State("MainMenu") {}
	void Enter();
	void Update(float deltaTime);
	void Render();
	void Resume() {}
	void Exit();
};