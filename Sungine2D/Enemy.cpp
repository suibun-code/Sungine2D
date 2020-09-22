#include "Enemy.h"

Enemy::Enemy(glm::vec2 position, ShaderUtil shaderUtil) : Entity(shaderUtil)
{
	mPosition = position;
}