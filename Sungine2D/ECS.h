#pragma once

#include <cstdint>
#include <bitset>
#include <queue>
#include <array>
#include <unordered_map>
#include <memory>
#include <set>

//GLM
#include "glm/glm/glm.hpp"

using ECSEntity = std::uint32_t;
using ECSComponentType = std::uint8_t;

const ECSEntity MAX_ENTITIES = 3000;
const ECSComponentType MAX_COMPONENTS = 32;

using Signature = std::bitset<MAX_COMPONENTS>;

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
		assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component added to same entity more than once.");

		//Put the new entry at the end and then update the maps.
		size_t newIndex = mSize;
		mEntityToIndexMap[entity] = newIndex;
		mIndexToEntityMap[newIndex] = entity;
		mComponentArray[newIndex] = component;
		++mSize;
	}

	void RemoveData(ECSEntity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Removing non-existent component.");

		//Copy element at end into deleted element's place to main density.
		size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
		size_t indexOfLastElement = mSize - 1;
		mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

		mEntityToIndexMap.erase(entity);
		mIndexToEntityMap.erase(indexOfLastElement);

		--mSize;
	}

	T& GetData(ECSEntity entity)
	{
		assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

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

class ECSComponentManager
{
private:
	//Map from type string pointer to a component type.
	std::unordered_map<const char*, ECSComponentType> mComponentTypes;

	//Map from type string pointer to a component array.
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> mComponentArrays;

	//The component type to be assigned to the next registered component - starting at 0.
	ECSComponentType mNextComponentType;

	//Convenience function to get the statically casted pointer to the ComponentArray of type T.
	template<typename T>
	std::shared_ptr<ECSComponentArray<T>> GetComponentArray()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		return std::static_pointer_cast<ECSComponentArray<T>>(mComponentArrays[typeName]);
	}

public:
	template<typename T>
	void RegisterComponent()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) == mComponentTypes.end() && "Registering component type more than once.");

		//Add this component type to the component type map.
		mComponentTypes.insert({ typeName, mNextComponentType });

		//Create a ComponentArray pointer and add it to the component arrays map.
		mComponentArrays.insert({ typeName, std::make_shared<ECSComponentArray<T>>() });

		//Increment the value so that the next component registered will be different.
		++mNextComponentType;
	}

	template<typename T>
	ECSComponentType GetComponentType()
	{
		const char* typeName = typeid(T).name();

		assert(mComponentTypes.find(typeName) != mComponentTypes.end() && "Component not registered before use.");

		//Return this component's type - used for creating signatures.
		return mComponentTypes[typeName];
	}

	template<typename T>
	void AddComponent(ECSEntity entity, T component)
	{
		//Add a component to the array for an entity.
		GetComponentArray<T>()->InsertData(entity, component);
	}

	template<typename T>
	void RemoveComponent(ECSEntity entity)
	{
		//Remove a component from the array for an entity.
		GetComponentArray<T>()->RemoveData(entity);
	}

	template<typename T>
	T& GetComponent(ECSEntity entity)
	{
		//Get a reference to a component from the array for an entity
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(ECSEntity entity)
	{
		//Notify each component array that an entity has been destroyed.
		//If it has a component for that entity, it will remove it.
		for (auto const& pair : mComponentArrays)
		{
			auto const& component = pair.second;

			component->EntityDestroyed(entity);
		}
	}
};

class ECSSystem
{
public:
	std::set<ECSEntity> mEntities;
};

class ECSSystemManager
{
private:
	//Map from system type string pointer to a signature.
	std::unordered_map<const char*, Signature> mSignatures;

	//Map from system type pointer to a system pointer.
	std::unordered_map<const char*, std::shared_ptr<ECSSystem>> mSystems;

public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) == mSystems.end() && "Registering system more than once.");

		//Create a pointer to the system and return it so it can be used externally.
		auto system = std::make_shared<T>();
		mSystems.insert({ typeName, system });
		return system;
	}

	template<typename T>
	void SetSignature(Signature signature)
	{
		const char* typeName = typeid(T).name();

		assert(mSystems.find(typeName) != mSystems.end() && "System used before being registered.");

		//Set the signature of this system.
		mSignatures.insert({ typeName, signature });
	}

	void EntityDestroyed(ECSEntity entity)
	{
		//Erase a destroyed entity from all system lists.
		//mEntities is a set so no check needed.
		for (auto const& pair : mSystems)
		{
			auto const& system = pair.second;

			system->mEntities.erase(entity);
		}
	}

	void EntitySignatureChanged(ECSEntity entity, Signature entitySignature)
	{
		//Notify each system that an entity's signature changed.

		for (auto const& pair : mSystems)
		{
			auto const& type = pair.first;
			auto const& system = pair.second;
			auto const& systemSignature = mSignatures[type];

			//Entity signature matches system signature - insert into set.
			if ((entitySignature & systemSignature) == systemSignature)
			{
				system->mEntities.insert(entity);
			}
			//Entity signature does not match system signature - erase from set.
			else
			{
				system->mEntities.erase(entity);
			}

		}
	}
};

class ECSHandler
{
private:
	std::unique_ptr<ECSComponentManager> mComponentManager;
	std::unique_ptr<ECSEntityManager> mEntityManager;
	std::unique_ptr<ECSSystemManager> mSystemManager;

public:
	void Init()
	{
		//Create pointers to each manager.
		mComponentManager = std::make_unique <ECSComponentManager>();
		mEntityManager = std::make_unique <ECSEntityManager>();
		mSystemManager = std::make_unique <ECSSystemManager>();
	}

	//***ENTITY METHODS***
	ECSEntity CreateEntity()
	{
		return mEntityManager->CreateEntity();
	}

	void DestroyEntity(ECSEntity entity)
	{
		mEntityManager->DestroyEntity(entity);
		mComponentManager->EntityDestroyed(entity);
		mSystemManager->EntityDestroyed(entity);
	}

	//***COMPONENT METHODS***
	template<typename T>
	void RegisterComponent()
	{
		mComponentManager->RegisterComponent<T>();
	}

	template<typename T>
	void AddComponent(ECSEntity entity, T component)
	{
		mComponentManager->AddComponent<T>(entity, component);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponentType<T>(), true);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	void RemoveComponent(ECSEntity entity)
	{
		mComponentManager->RemoveComponent<T>(entity);

		auto signature = mEntityManager->GetSignature(entity);
		signature.set(mComponentManager->GetComponent<T>(), false);
		mEntityManager->SetSignature(entity, signature);

		mSystemManager->EntitySignatureChanged(entity, signature);
	}

	template<typename T>
	T& GetComponent(ECSEntity entity)
	{
		return mComponentManager->GetComponent<T>(entity);
	}

	template<typename T>
	ECSComponentType GetComponentType()
	{
		return mComponentManager->GetComponentType<T>();
	}

	//***SYSTEM METHODS***
	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return mSystemManager->RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		mSystemManager->SetSignature<T>(signature);
	}
};

struct Transform
{
	float scale = 1.f;
	float rotation = 0.f;
	glm::vec2 position = glm::vec2(0.f, 0.f);
	glm::vec2 size = glm::vec2(1.f, 1.f);
};

