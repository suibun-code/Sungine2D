#pragma once

#include <queue>
#include <array>
#include <cassert>

#include "Types.h"

class ECSEntityManager
{
private:
	//Queue of unused IDs.
	std::queue<ECSEntity> mAvailableEntities;

	//Array of signatures where the index corresponds to the entity ID.
	std::array<Signature, MAX_ENTITIES> mSignatures;

	//Total active entities. Used to keep a limit on how many can be active.
	ECSEntity mLivingEntityCount{};

public:
	ECSEntityManager()
	{
		for (ECSEntity entity = 0; entity < MAX_ENTITIES; ++entity)
			mAvailableEntities.push(entity);
	}

	ECSEntity CreateEntity()
	{
		assert(mLivingEntityCount < MAX_ENTITIES && "Too many entities exist.");

		//Take an ID from the front of the queue.
		ECSEntity id = mAvailableEntities.front();
		mAvailableEntities.pop();
		++mLivingEntityCount;

		return id;
	}

	void DestroyEntity(ECSEntity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		//Invalidate the destroyed entity's signature.
		mSignatures[entity].reset();

		//Put the destroyed ID at the back of the queue.
		mAvailableEntities.push(entity);
		--mLivingEntityCount;
	}

	void DestroyAllEntities()
	{
		for (uint32_t i = 0; i < mSignatures.size(); i++)
		{
			mSignatures[i].reset();
			mLivingEntityCount = 0;
		}

		for (ECSEntity entity = 0; entity < MAX_ENTITIES; ++entity)
			mAvailableEntities.push(entity);
	}

	void SetSignature(ECSEntity entity, Signature signature)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		//Put this entity's signature into the array.
		mSignatures[entity] = signature;
	}

	Signature GetSignature(ECSEntity entity)
	{
		assert(entity < MAX_ENTITIES && "Entity out of range.");

		//Get this entit's signature from the array.
		return mSignatures[entity];
	}
};