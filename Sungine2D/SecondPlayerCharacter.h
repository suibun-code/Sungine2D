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
	int player = 0;
	Bullet* tempBullet = nullptr;
	std::queue<Bullet*> mBulletOP;
	float rotation = 0.f;
	glm::vec2 position = glm::vec2(0.f);

public:
	SecondPlayerCharacter() {}
	SecondPlayerCharacter(int player, glm::vec2 position);

	void Start() override;
	void Update(float deltaTime) override;
	void Destroy() override;

	bool OnCollision(ECSEntity other) override;
};
