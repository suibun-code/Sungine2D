#pragma once

#include "ECSSystem.h"
#include "BehaviourScript.h"
#include "EntityData.h"
#include "ECSHandler.h"

//GLM
#include "glm/glm/glm.hpp"

struct Collider
{
	bool moveable = false;
	bool overlapper = false;
	glm::vec2 boundingBox = glm::vec2(1.f);
	glm::vec2 offset = glm::vec2(0.f);
	bool colliding = false;

	bool Call(ECSEntity other, ECSEntity entity)
	{
		return ECSHandler::Instance()->GetComponent<EntityData>(entity).script->OnCollision(other);
	}
};