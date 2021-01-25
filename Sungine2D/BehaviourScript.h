#pragma once
#include <vector>

class BehaviourScript
{
protected:
	static std::vector<BehaviourScript*> mAllBehaviourScripts;
	
public:
	BehaviourScript();
	~BehaviourScript();

	void AddToAll();
	
	virtual void Start();
	virtual void Update();

	static std::vector<BehaviourScript*> GetBehaviourScripts() { return mAllBehaviourScripts; }
};
