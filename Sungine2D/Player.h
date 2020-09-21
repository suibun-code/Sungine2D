#pragma once

#include "SuSprite.h"

class Player : public SuSprite
{
private:
	int mMaxHealt = 100, mHealth = 100;
	int mDirection = 1; //Negative for left, positive for right.
	glm::vec2 mPosition = { 0, 0 };
	float mSpeed = 0.5f;

public:
	Player(glm::vec2 position, ShaderUtil shaderUtil);

	void SetHealth(int health) { mHealth = health; }
	int GetHealth() { return mHealth; }

	void MoveX(float deltaTime);
	void MoveY(float deltaTime);

	void SetDirection(int direction) { mDirection = direction; }

	glm::vec2 GetPosition() { return mPosition; }
};

