#include "MovementSystem.h"

#include <iostream>

#include "ECSHandler.h"
#include "Core.h"
#include "TransformComponent.h"

extern ECSHandler handler;

void MovementSystem::Init()
{
}

void MovementSystem::Update(float deltaTime)
{
	for (auto const& entity : mEntities)
	{
		auto& transform = handler.GetComponent<TransformComponent>(entity);

		transform.position.x += 1;

		if (Core::Instance()->KeyDown(SDL_SCANCODE_R))
		{
			std::cout << "WONDABA!\n";
		}
	}
}

