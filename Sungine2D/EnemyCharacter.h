#pragma once

#include "BehaviourScript.h"
#include "Types.h"

class EnemyCharacter : public BehaviourScript
{
private:
	ECSEntity mEntity;
	
	int health = 100;
	ECSEntity healthText;
	
public:
	EnemyCharacter();

	void Start() override;
	void Destroy() override;
	void Update(float deltaTime) override;

	ECSEntity GetEntity();
	bool OnCollision(ECSEntity other) override;
	
};
