#pragma once

#include "SuSprite.h"
#include "ShaderUtil.h"
#include "Transform.h"

class Entity : public SuSprite
{
private:
	static int msGameObjectCount;

protected:
	Transform transform;

public:
	Entity(ShaderUtil& shader);
	~Entity();

	void SetPosition(glm::vec2 position) { transform.position = position; }
	void SetScale(glm::vec2 scale) { transform.scale = scale; }
	void SetRotation(float rotation) { transform.rotation = rotation; }

	glm::vec2 GetPosition() { return transform.position; }
	glm::vec2 GetScale() { return transform.scale; }
	float GetRotation() { return transform.rotation; }
};

