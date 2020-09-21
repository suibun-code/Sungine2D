#include "Enemy.h"

Enemy::Enemy(glm::vec2 position, ShaderUtil shaderUtil) : SuSprite(shaderUtil)
{
	mPosition = position;
}