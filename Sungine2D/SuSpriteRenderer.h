#pragma once

#include "ShaderUtil.h"
#include "SuTexture2D.h"

namespace Sungine
{
	class SuSpriteRenderer
	{
	private:
		//Shader variable for the shader from the resource manager.
		ShaderUtil mShaderUtil;

		unsigned int mQuadVAO, IBO, mVBO, mTVBO;

		//Initializes VAO, IBO and various VBO's.
		void initRenderData();

	public:
		SuSpriteRenderer(ShaderUtil& shader);

		~SuSpriteRenderer();

		//Renders the sprite with the given parameters.
		void DrawSprite(SuTexture2D& texture, glm::vec2 position, glm::vec2 size = glm::vec2(1.0f, 1.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

		void SetAlpha(float alpha);
	};
}

