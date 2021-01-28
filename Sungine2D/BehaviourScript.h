#pragma once
#include <vector>

#include "Collision.h"
#include "ECSSystem.h"

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
	virtual void Update(float deltaTime) = 0;

	//Return true if the owning entity was destroyed in this method.
	virtual bool OnCollision(ECSEntity other);

	static std::vector<BehaviourScript*> GetBehaviourScripts() { return mAllBehaviourScripts; }
};
