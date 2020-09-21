#pragma once

#include "SuSprite.h"

class Enemy : public SuSprite
{
private:
	int mMaxHealth = 100, mHealth = 100;
	glm::vec2 mPosition = { 0, 0 };
	float mSpeed = 0.5f;

public:
	Enemy(glm::vec2 position, ShaderUtil shaderUtil);

	void SetHealth(int health) { mHealth = health; }
	int GetHealth() { return mHealth; }

	glm::vec2 GetPosition() { return mPosition; }
};

