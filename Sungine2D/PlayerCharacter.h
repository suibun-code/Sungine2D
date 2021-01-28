﻿#pragma once

#include "ECSHandler.h"
#include "Bullet.h"

class PlayerCharacter : public BehaviourScript
{
private:
	ECSEntity mEntity;
	std::vector<Bullet*> mBulletOP;

public:
	PlayerCharacter();

	void Start() override;
	void Destroy() override;
	void Update(float deltaTime) override;

	ECSEntity GetEntity();
	bool OnCollision(ECSEntity other) override;
};
