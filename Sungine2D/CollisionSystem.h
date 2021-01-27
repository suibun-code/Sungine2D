#pragma once

#include "ECSSystem.h"

#include "ECSHandler.h"

enum class HighestDepth
{
	RIGHT,
	LEFT,
	TOP,
	BOTTOM
};

class CollisionSystem : public ECSSystem
{
private:
	float xDepth = 0.f;
	float yDepth = 0.f;
	HighestDepth xHighestDepth = HighestDepth::RIGHT;
	HighestDepth yHighestDepth = HighestDepth::BOTTOM;

public:
	void Init();
	void Update();
	bool OnCollision(ECSEntity entity, ECSEntity other);
};

