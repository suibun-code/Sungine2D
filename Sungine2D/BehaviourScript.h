#pragma once
#include <vector>

class BehaviourScript
{
protected:
	static std::vector<BehaviourScript*> mAllBehaviourScripts;
	
public:
	BehaviourScript();
	~BehaviourScript();

	void Generate();
	
	virtual void Start() = 0;
	virtual void Destroy() = 0;
	virtual void Update() = 0;

	static std::vector<BehaviourScript*> GetBehaviourScripts() { return mAllBehaviourScripts; }
};
