#include "Player.h"

Player::Player(SuTexture2D sprite, glm::vec2 pos, glm::vec2 size) : Entity(sprite, pos, size)
{
	SetName("Player");
	mTransform.position = pos;
}

void Player::MoveX(float deltaTime)
{
	mTransform.position.x += (mSpeed * deltaTime) * mDirection;
}

void Player::MoveY(float deltaTime)
{
	mTransform.position.y += (mSpeed * deltaTime) * mDirection;
}

void Player::SetDirection(int direction)
{ 
	mDirection = direction;
}
