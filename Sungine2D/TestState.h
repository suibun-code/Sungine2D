#pragma once

#include "FSM.h"

#include "Player.h"
#include "Enemy.h"

//Components
#include "TransformComponent.h"
#include "RenderComponent.h"

//Systems
#include "MovementSystem.h"
#include "RenderSystem.h"

//ECSHandler
#include "ECSHandler.h"

class TestState : public State
{
private:
	Enemy* enemy;
	Player* player;

	//ECSHandler handler;
	//std::shared_ptr<MovementSystem> movementSystem;
	//std::shared_ptr<RenderSystem> renderSystem;

public:
	TestState() : State("Test") {}
	void Enter();
	void Update(float deltaTime);
	void Render();
	void Resume() {}
	void Exit();
};
