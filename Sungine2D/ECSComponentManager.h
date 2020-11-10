#pragma once

#include <unordered_map>
#include <memory>

#include "Types.h"
#include "ECSComponentArray.h"

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

	template<typename T>
	bool HasComponent(ECSEntity entity)
	{
		return GetComponentArray<T>()->HasData(entity);
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