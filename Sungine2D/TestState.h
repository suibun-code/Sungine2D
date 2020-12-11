#pragma once

#include "FSM.h"

#include "ECSEntityManager.h"

class TestState : public State
{
private:

public:
	TestState() : State("Test") {}
	void Enter();
	void HandleStateEvents(SDL_Event* event);
	void Update(float deltaTime);
	void Render();
	void LateUpdate(float deltaTime);
	void Resume() {}
	void Exit();
};