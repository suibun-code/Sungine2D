#include "MovementSystem.h"

#include <iostream>

#include "Core.h"
#include "Transform.h"
#include "Movement.h"
#include "Collider.h"

void MovementSystem::Init()
{

}

void MovementSystem::Update(float deltaTime)
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entity);
		auto& movement = ECSHandler::Instance()->GetComponent<Movement>(entity);

		if (transform.IsDirty() || movement.velocity.x != 0 || movement.velocity.y != 0)
			transform.AddPosition(movement.velocity * deltaTime);
	}
}

