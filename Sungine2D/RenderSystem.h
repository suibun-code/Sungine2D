#pragma once

#include "ECSSystem.h"

#include "ShaderUtil.h"
#include "SuTexture2D.h"

class RenderSystem : public ECSSystem
{
private:
	unsigned int mQuadVAO, IBO, mVBO, mTVBO;

public:
	//Initializes VAO, IBO and various VBO's.
	void Init();

	//Draws the texture.
	void Draw();
};

