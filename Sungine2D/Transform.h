#pragma once

//GLM
#include "glm/glm/glm.hpp"

struct Transform
{
	glm::vec2 position = glm::vec2(0.f, 0.f);
	glm::vec2 size = glm::vec2(1.f, 1.f);
	float rotation = 0.f;
	float scale = 1.f;
	bool dirty = true;

	bool IsDirty() { return dirty; }

	void AddPosition(glm::vec2 position) 
	{
		this->position += position;
		dirty = true; 
	}

	void SetPosition(glm::vec2 position) 
	{
		this->position = position;
		dirty = true; 
	}

	void SetSize(glm::vec2 size) 
	{ 
		this->size = size;
		dirty = true; 
	}

	void SetRotation(float rotation) 
	{ 
		this->rotation = rotation;
		dirty = true; 
	}

	void SetScale(float scale) 
	{ 
		this->scale = scale;
		dirty = true; 
	}
};
