#pragma once

#include "FSM.h"

#include "ECSEntityManager.h"

class TwoPlayerMode : public State
{
public:
	TwoPlayerMode() : State("TwoPlayer") {}
	void Enter();
	void HandleStateEvents(SDL_Event* event);
	void Update(float deltaTime);
	void Render();
	void LateUpdate(float deltaTime);
	void Resume() {}
	void Exit();
};
