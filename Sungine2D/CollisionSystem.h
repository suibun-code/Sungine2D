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
	float depth = 0.f;
	HighestDepth highestDepth = HighestDepth::RIGHT;

public:
	void Init();
	void Update();
};

