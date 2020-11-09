#include "MovementSystem.h"

#include <iostream>

#include "Core.h"
#include "TransformComponent.h"
#include "MovementComponent.h"

void MovementSystem::Init()
{
}

void MovementSystem::Update(float deltaTime)
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<TransformComponent>(entity);
		auto& movement = ECSHandler::Instance()->GetComponent<MovementComponent>(entity);

		if (Core::Instance()->KeyDown(SDL_SCANCODE_A))
		{
			transform.position.x -= movement.speed * deltaTime;
		}
		if (Core::Instance()->KeyDown(SDL_SCANCODE_D))
		{
			transform.position.x += movement.speed * deltaTime;
		}
		if (Core::Instance()->KeyDown(SDL_SCANCODE_W))
		{
			transform.position.y -= movement.speed * deltaTime;
		}
		if (Core::Instance()->KeyDown(SDL_SCANCODE_S))
		{
			transform.position.y += movement.speed * deltaTime;
		}
	}
}

