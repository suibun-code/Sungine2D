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
	std::cout << "Behaviour scripts size: " << mAllBehaviourScripts.size() << "\n";

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

void BehaviourScript::SetParent(BehaviourScript* parent)
{
	mOwningObject = parent;
}

ECSEntity BehaviourScript::GetEntity()
{
	return mEntity;
}

bool BehaviourScript::OnCollision(ECSEntity other)
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


