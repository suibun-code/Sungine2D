#include "BehaviourScript.h"

#include <iostream>

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
	std::cout << mAllBehaviourScripts.size() << "\n";

	if (!mAllBehaviourScripts.empty())
		mAllBehaviourScripts.back()->Start();
}

bool BehaviourScript::OnCollision(ECSEntity other)
{
	return false;
}


