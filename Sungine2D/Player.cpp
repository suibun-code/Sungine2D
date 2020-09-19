#include "Player.h"

Player::Player(glm::vec2 position, ShaderUtil shaderUtil) : SuSprite(shaderUtil)
{
	mPosition = position;
}

void Player::MoveX(float deltaTime)
{
	mPosition.x += (mSpeed * deltaTime) * mDirection;
}

void Player::MoveY(float deltaTime)
{
	mPosition.y += (mSpeed * deltaTime) * mDirection;
}

void Player::SetDirection(int direction)
{
	mDirection = direction;
}
