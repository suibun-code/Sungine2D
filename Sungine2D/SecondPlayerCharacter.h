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

class SecondPlayerCharacter : public BehaviourScript
{
private:
	Bullet* tempBullet = nullptr;
	std::queue<Bullet*> mBulletOP;
	int player = 0;

	float rotation = 0.f;

	

public:
	SecondPlayerCharacter() {}
	SecondPlayerCharacter(int player);

	void Start() override;
	void Update(float deltaTime) override;
	void Destroy() override;

	bool OnCollision(ECSEntity other) override;
};
