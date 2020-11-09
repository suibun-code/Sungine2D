#pragma once

#include <assert.h>

#include "Types.h"

#include <iostream>

class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(ECSEntity entity) = 0;
};

template<typename T>
class ECSComponentArray : public IComponentArray
{
private:
	//Packed array of components. Every entity has a unique spot (Same limit).
	std::array<T, MAX_ENTITIES> mComponentArray;

	//Map from an entity to an index, and map from an index to an entity.
	std::unordered_map<ECSEntity, size_t> mEntityToIndexMap;
	std::unordered_map<size_t, ECSEntity> mIndexToEntityMap;

	//Total size of valid values in the array.
	size_t mSize;

public:
	void InsertData(ECSEntity entity, T component)
	{
		//assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

		//Put the new entry at the end and then update the maps.
		size_t newIndex = mSize;
		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;
		mComponentArray[newIndex] = component;
		++mSize;
	}

	void RemoveData(ECSEntity entity)
	{
		//assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

		//Copy element at end into deleted element's place to main density.
		size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
		size_t indexOfLastElement = mSize - 1;
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

		// Update map to point to moved spot
		ECSEntity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
		mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
		mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(indexOfLastElement);

		--mSize;
	}

	T& GetData(ECSEntity entity)
	{
		//assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

		//Return a reference to the entity's component;
		return mComponentArray[mEntityToIndexMap[entity]];
	}

	void EntityDestroyed(ECSEntity entity) override
	{
		if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
		{
			//Remove the entity's component if it existed
			RemoveData(entity);
		}
	}
};