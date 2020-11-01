#pragma once

#include "SuTexture2D.h"

struct RenderComponent
{
	ShaderUtil shaderUtil;
	SuTexture2D texture;
	glm::vec3 color;
};