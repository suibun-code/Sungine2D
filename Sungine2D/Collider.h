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
	bool solid = false;
	bool trigger = false;
	glm::vec2 boundingBox = glm::vec2(1.f);
	glm::vec2 offset = glm::vec2(0.f);
	bool colliding = false;
	ECSEntity other = NULL;

	bool Call(ECSEntity other, ECSEntity entity)
	{
		this->other = other;
		return ECSHandler::Instance()->GetComponent<EntityData>(entity).script->OnCollision(other);
	}
};