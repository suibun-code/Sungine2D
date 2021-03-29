#pragma once

#include <SDL_stdinc.h>
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
	virtual ~BehaviourScript();

	void Generate();
	
	virtual void Start() = 0;
	virtual void Destroy();
	virtual void Update(float deltaTime) = 0;

	void SetOwner(BehaviourScript* owner);

	virtual ECSEntity GetEntity();

	//Return true if the owning entity was destroyed in this method.
	virtual bool OnCollision(ECSEntity other);
	virtual bool OnRender(ECSEntity entity);

	static std::vector<BehaviourScript*> GetBehaviourScripts() { return mAllBehaviourScripts; }
	static void ClearBehaviourScripts();
};
