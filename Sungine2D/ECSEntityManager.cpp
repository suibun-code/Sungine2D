#include "ECSEntityManager.h"

#include "ECSHandler.h"
#include "EntityData.h"

#include "Core.h"

ECSEntity ECSEntityManager::CreateEntity()
{

	//assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities exist.");

	//Take an ID from the front of the queue.
	ECSEntity id = mAvailableEntities.front();
	mAvailableEntities.pop();
	++mLivingEntityCount;

	ECSHandler::Instance()->AddComponent<EntityData>(id, EntityData{ "Entity" + std::to_string(id), "General" });
	Core::Instance()->GetCurrentState()->AddEntity(id);

	return id;
}
