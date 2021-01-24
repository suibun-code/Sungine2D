#pragma once
#include <vector>

class BehaviourScript
{
private:
	std::vector<BehaviourScript*> mAllBehaviourScripts;
	
public:
	BehaviourScript();
	~BehaviourScript();
	
	void Start();
	void Update();
};
