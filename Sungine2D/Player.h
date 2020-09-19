#pragma once

#include "SuSprite.h"

class Player : public SuSprite
{
private:
	glm::vec2 mPosition = { 0, 0 };
	int mDirection = 1; //Negative for left, positive for right.
	float mSpeed = 0.5f;

public:
	Player(glm::vec2 position, ShaderUtil shaderUtil);

	void MoveX(float deltaTime);
	void MoveY(float deltaTime);

	void SetDirection(int direction);

	glm::vec2 GetPosition() { return mPosition; }
};

