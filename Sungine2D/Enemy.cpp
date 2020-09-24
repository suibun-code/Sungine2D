#include "Enemy.h"

Enemy::Enemy(SuTexture2D sprite, glm::vec2 pos, glm::vec2 size) : Entity(sprite, pos, size)
{
	mTransform.position = pos;
}