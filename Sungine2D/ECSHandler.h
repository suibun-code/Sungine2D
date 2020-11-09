#pragma once

#include <memory>

#include "Types.h"
#include "ECSComponentManager.h"
#include "ECSEntityManager.h"
#include "ECSSystemManager.h"

class ECSHandler
{
private:
	static ECSHandler* mspHandlerInstance;

	std::unique_ptr<ECSComponentManager> mComponentManager;
	std::unique_ptr<ECSEntityManager> mEntityManager;
	std::unique_ptr<ECSSystemManager> mSystemManager;

public:
	//Singleton instance of ECSHandler.
	static ECSHandler* Instance()
	{
		if (mspHandlerInstance == nullptr)
		{
			std::cout << "Made new ECSHandler.\n";
			mspHandlerInstance = new ECSHandler();
		}
		return mspHandlerInstance;
	}

	static void ResetInstance()
	{
		delete mspHandlerInstance;
		mspHandlerInstance = nullptr;
	}

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

	void DestroyEntity(ECSEntity entity);

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

	ECSEntity ActiveEntityCount()
	{
		return mEntityManager->ActiveEntityCount();
	}
};