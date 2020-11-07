#pragma once

#include "FSM.h"

class MainMenu :  public State
{
private:

public:
	MainMenu() : State("MainMenu") {}
	void Enter();
	void Update(float deltaTime);
	void Render();
	void Resume() {}
	void Exit();
};