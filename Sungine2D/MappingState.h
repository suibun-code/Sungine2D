#pragma once

#include "FSM.h"

#include "ECSEntityManager.h"

class MappingState : public State
{
private:

public:
	MappingState() : State("MappingState") {}
	void Enter();
	void Update(float deltaTime);
	void Render();
	void Resume() {}
	void Exit();
};

