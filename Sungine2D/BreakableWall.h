#pragma once

#include "BehaviourScript.h"
#include "SuTexture2D.h"

class BreakableWall : public BehaviourScript
{
public:
	SuTexture2D mTexture;

	BreakableWall();
	~BreakableWall();

	void Start() override;
	void Destroy() override;
	void Update(float deltaTime) override;

	bool OnCollision(ECSEntity other) override;
};

