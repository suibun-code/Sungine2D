#pragma once

#include "ECSSystem.h"

class MovementSystem : public ECSSystem
{
public:
	void Init();

	void Update(float deltaTime);
};
