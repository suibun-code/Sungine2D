#pragma once

#include "SuTexture2D.h"
#include "ShaderUtil.h"
#include "ECSHandler.h"
#include "EntityData.h"

struct Rendering
{
	ShaderUtil shaderUtil;
	SuTexture2D texture = SuTexture2D();
	glm::vec3 color = glm::vec3(1.f);
	glm::mat4 model = glm::mat4(1.0f);

	bool Call(ECSEntity entity)
	{
		return ECSHandler::Instance()->GetComponent<EntityData>(entity).script->OnRender(entity);
	}
};