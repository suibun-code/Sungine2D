#pragma once

#include "ECSHandler.h"
#include "Bullet.h"

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
