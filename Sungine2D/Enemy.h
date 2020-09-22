#pragma once

#include "Entity.h"

class Enemy : public Entity
{
private:
	int mMaxHealth = 100, mHealth = 100;
	float mSpeed = 0.5f;

public:
	Enemy(glm::vec2 position, ShaderUtil shaderUtil);

	void SetHealth(int health) { mHealth = health; }
	int GetHealth() { return mHealth; }
};

