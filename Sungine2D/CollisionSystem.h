#pragma once

#include "ECSSystem.h"

#include "ECSHandler.h"

class CollisionSystem : public ECSSystem
{
public:
	void Init();
	void Update();
};

