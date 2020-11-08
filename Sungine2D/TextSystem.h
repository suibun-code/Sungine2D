#pragma once

#include "ECSSystem.h"

#include "ECSHandler.h"

#include "ShaderUtil.h"
#include "SuTexture2D.h"

class TextSystem : public ECSSystem
{
public:
	void Init();
	void Update();
};

