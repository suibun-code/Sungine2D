#pragma once

#include <chrono>
#include <ctime>

#include "ECSHandler.h"
#include "Bullet.h"

#include "Transform.h"
#include "Rendering.h"
#include "Movement.h"
#include "Collider.h"
#include "Character.h"

class PlayerCharacter : public BehaviourScript
{
private:
	Bullet* testbullet = nullptr;
	std::queue<Bullet*> mBulletOP;

public:
	PlayerCharacter();

	void Start() override;
	void Destroy() override;
	void Update(float deltaTime) override;
	
	bool OnCollision(ECSEntity other) override;
};
