#include "BehaviourScript.h"

#include <iostream>

#include "ECSHandler.h"

std::vector<BehaviourScript*> BehaviourScript::mAllBehaviourScripts;

BehaviourScript::BehaviourScript()
{
	
}

BehaviourScript::~BehaviourScript()
{
}

void BehaviourScript::Generate()
{
	mAllBehaviourScripts.push_back(this);

	if (!mAllBehaviourScripts.empty())
		mAllBehaviourScripts.back()->Start();
}

void BehaviourScript::Destroy()
{
	for (unsigned int i = 0; i < mAllBehaviourScripts.size(); i++)
		if (this == mAllBehaviourScripts.at(i))
			mAllBehaviourScripts.erase(mAllBehaviourScripts.begin() + i);

	ECSHandler::Instance()->DestroyEntity(mEntity);
	delete mOwningObject;
	mOwningObject = nullptr;
}

void BehaviourScript::SetOwner(BehaviourScript* owner)
{
	mOwningObject = owner;
}

ECSEntity BehaviourScript::GetEntity()
{
	return mEntity;
}

bool BehaviourScript::OnCollision(ECSEntity other)
{
	return false;
}

bool BehaviourScript::OnRender(ECSEntity entity)
{
	return false;
}


void BehaviourScript::ClearBehaviourScripts()
{
	for (auto script : mAllBehaviourScripts)
	{
		delete script;
		script = nullptr;
	}

	mAllBehaviourScripts.clear();
}


