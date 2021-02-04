#pragma once

#include "BehaviourScript.h"
#include "Types.h"

class EnemyCharacter : public BehaviourScript
{
private:

	
public:
	EnemyCharacter();

	void Start() override;
	void Destroy() override;
	void Update(float deltaTime) override;
	
	bool OnCollision(ECSEntity other) override;
	
};
