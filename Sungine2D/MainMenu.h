#pragma once

#include "FSM.h"

#include "ECSEntityManager.h"

class MainMenu :  public State
{
private:

public:
	MainMenu() : State("MainMenu") {}
	void Enter();
	void HandleStateEvents(SDL_Event* event);
	void Update(float deltaTime);
	void Render();
	void LateUpdate(float deltaTime);
	void Resume() {}
	void Exit();
};