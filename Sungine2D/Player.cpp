#include "Player.h"

Player::Player(glm::vec2 position, ShaderUtil shaderUtil) : Entity(shaderUtil)
{
	transform.position = position;
}

void Player::MoveX(float deltaTime)
{
	transform.position.x += (mSpeed * deltaTime) * mDirection;
}

void Player::MoveY(float deltaTime)
{
	transform.position.y += (mSpeed * deltaTime) * mDirection;
}

void Player::SetDirection(int direction)
{ 
	if (mDirection != direction)
	{
		
	}

	mDirection = direction;
}
