#pragma once

#include "ECSSystem.h"

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

