#pragma once

#include "FSM.h"

#include "Player.h"
#include "Enemy.h"

#include "ECSEntityManager.h"

class TestState : public State
{
private:

public:
	TestState() : State("Test") {}
	void Enter();
	void Update(float deltaTime);
	void Render();
	void Resume() {}
	void Exit();
};