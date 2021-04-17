#pragma once

#include "FSM.h"

class WinState : public State
{
private:
	std::string whoWon;

public:
	WinState(std::string whoWon) : State("WinState") { this->whoWon = whoWon; }
	void Enter();
	void HandleStateEvents(SDL_Event* event);
	void Update(float deltaTime);
	void Render();
	void LateUpdate(float deltaTime);
	void Resume() {}
	void Exit();
};

