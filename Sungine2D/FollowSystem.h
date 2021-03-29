#pragma once

#include <SDL_stdinc.h>

#include "ECSSystem.h"

class FollowSystem : public ECSSystem
{
private:

public:
	void Init();
	void Update(float deltaTime);
	
};
