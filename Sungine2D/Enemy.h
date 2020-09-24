#pragma once

#include "Entity.h"

class Enemy : public Entity
{
private:
	int mMaxHealth = 100, mHealth = 100;
	float mSpeed = 0.5f;

public:
	Enemy(SuTexture2D sprite, glm::vec2 pos, glm::vec2 size = glm::vec2(1.f));

	void SetHealth(int health) { mHealth = health; }
	int GetHealth() { return mHealth; }
};

