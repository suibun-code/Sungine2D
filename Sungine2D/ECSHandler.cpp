#include "ECSHandler.h"

#include "Core.h"

void ECSHandler::DestroyEntity(ECSEntity entity)
{
	mEntityManager->DestroyEntity(entity);
	mComponentManager->EntityDestroyed(entity);
	mSystemManager->EntityDestroyed(entity);

	Core::Instance()->GetCurrentState()->RemoveEntity(entity);
}