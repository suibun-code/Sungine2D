#pragma once

#include "ECSSystem.h"

#include "ECSHandler.h"

#include "ShaderUtil.h"
#include "SuTexture2D.h"

class TextSystem : public ECSSystem
{
private:
	unsigned int mQuadVAO, IBO, mVBO, mTVBO;

public:
	void Init();
	void Update();
};

