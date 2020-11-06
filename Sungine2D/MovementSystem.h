#pragma once

#include "ECSSystem.h"

#include "ECSHandler.h"

class MovementSystem : public ECSSystem
{
private:

public:
	void Init();

	void Update(float deltaTime);
};
