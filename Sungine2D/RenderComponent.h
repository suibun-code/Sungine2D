#pragma once

#include "SuTexture2D.h"
#include "ShaderUtil.h"

struct RenderComponent
{
	ShaderUtil shaderUtil;
	SuTexture2D texture = SuTexture2D();
	glm::vec3 color = glm::vec3(1.f);
};