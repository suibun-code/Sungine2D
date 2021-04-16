#pragma once

#include "FSM.h"

class PauseState : public State
{
public:
	PauseState() : State("PauseState") {}
	void Enter();
	void HandleStateEvents(SDL_Event* event);
	void Update(float deltaTime);
	void Render();
	void LateUpdate(float deltaTime);
	void Resume() {}
	void Exit();
};

