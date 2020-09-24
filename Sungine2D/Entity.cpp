#include "Entity.h"

int Entity::msGameObjectCount = 0;

Entity::Entity()
{
	msGameObjectCount++;
	
	mTransform.size = mTransform.size * mTransform.scale;
}

Entity::Entity(SuTexture2D sprite, glm::vec2 pos, glm::vec2 size)
{
	msGameObjectCount++;

	mTexture = sprite;
	mTransform.position = pos;
	mTransform.size = glm::vec2(sprite.Width, sprite.Height) * mTransform.scale;
}

Entity::~Entity()
{
	msGameObjectCount--;
}

void Entity::Draw(SuSprite& renderer)
{
	renderer.DrawSprite(mTexture, mTransform.position, mTransform.size);
}
