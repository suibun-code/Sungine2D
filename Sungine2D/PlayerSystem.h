#pragma once

#include <vector>

#include "ECSSystem.h"
#include "SuTexture2D.h"

class PlayerSystem : public ECSSystem
{
private:
	SuTexture2D texture;
	std::vector<ECSEntity> bullets;

public:
	void Init();
	void Update(float deltaTime);
};

