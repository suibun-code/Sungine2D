#include "Entity.h"

int Entity::msGameObjectCount = 0;

Entity::Entity(ShaderUtil& shader) : SuSprite(shader)
{
	msGameObjectCount++;
}

Entity::~Entity()
{
	msGameObjectCount--;
}
