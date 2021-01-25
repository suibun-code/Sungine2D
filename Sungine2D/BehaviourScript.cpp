#include "BehaviourScript.h"

#include <iostream>

std::vector<BehaviourScript*> BehaviourScript::mAllBehaviourScripts;

BehaviourScript::BehaviourScript()
{
	AddToAll();
}

BehaviourScript::~BehaviourScript()
{
}

void BehaviourScript::AddToAll()
{
	mAllBehaviourScripts.push_back(this);

	std::cout << mAllBehaviourScripts.size() << "\n";
}

void BehaviourScript::Start()
{
	if (!mAllBehaviourScripts.empty())
		mAllBehaviourScripts.back()->Start();
}

void BehaviourScript::Update()
{
}
