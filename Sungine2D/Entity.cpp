#include "Entity.h"

std::vector<Entity*> Entity::msAllEntities;
int Entity::msGameObjectCount = 0;

Entity::Entity() : mName("NoName")
{
	mEntityID = msGameObjectCount;
	msAllEntities.push_back(this);
	msGameObjectCount++;

	mTransform.size = mTransform.size * mTransform.scale;
}

Entity::Entity(SuTexture2D sprite, glm::vec2 pos, glm::vec2 size) : mName("NoName")
{
	mEntityID = msGameObjectCount;
	msAllEntities.push_back(this);
	msGameObjectCount++;

	mTexture = sprite;
	mTransform.position = pos;
	mTransform.size = glm::vec2(sprite.Width, sprite.Height) * mTransform.scale;
}

Entity::~Entity()
{
	if (msAllEntities.size() > 1)
		if (msAllEntities.at(mEntityID) != msAllEntities.back())
		{
			//Give the object at the back of the vector the ID of this object which is currently being destroyed. Otherwise, it will remain in the same position in terms of the mEntityID variable.
			msAllEntities.back()->mEntityID = mEntityID;

			//Actually swap the objects in the vector. We do not need to change the mEntityID of this object as it is being destroyed anyways.
			std::swap(msAllEntities.at(mEntityID), msAllEntities.back());
		}

	//Remove the last element in the vector.
	msAllEntities.pop_back();
	msGameObjectCount--;
}

void Entity::Draw(SuSprite& renderer)
{
	renderer.DrawSprite(mTexture, mTransform.position, mTransform.size);
}
