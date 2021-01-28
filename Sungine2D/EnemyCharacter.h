#pragma once

#include "BehaviourScript.h"
#include "Types.h"

class EnemyCharacter : public BehaviourScript
{
private:
	int health = 100;
	ECSEntity healthText;
	
public:
	EnemyCharacter();

	void Start() override;
	void Destroy() override;
	void Update(float deltaTime) override;
	
	bool OnCollision(ECSEntity other) override;
	
};
