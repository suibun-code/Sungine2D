#pragma once

#include "SuTexture2D.h"
#include "ShaderUtil.h"

struct RenderComponent
{
	ShaderUtil shaderUtil;
	SuTexture2D texture;
	glm::vec3 color;
};