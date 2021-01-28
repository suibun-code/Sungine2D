#pragma once

#include <vector>

#include "ECSSystem.h"

class BehaviourScript
{
protected:
	BehaviourScript* mOwningObject;
	ECSEntity mEntity;
	static std::vector<BehaviourScript*> mAllBehaviourScripts;
	
public:
	BehaviourScript();
	~BehaviourScript();

	void Generate();
	
	virtual void Start() = 0;
	virtual void Destroy();
	virtual void Update(float deltaTime) = 0;

	void SetParent(BehaviourScript* parent);

	virtual ECSEntity GetEntity();

	//Return true if the owning entity was destroyed in this method.
	virtual bool OnCollision(ECSEntity other);

	static std::vector<BehaviourScript*> GetBehaviourScripts() { return mAllBehaviourScripts; }
};
