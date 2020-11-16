#include "PlayerSystem.h"

#include <math.h>

#include "Transform.h"
#include "Rendering.h"

#include "Core.h"
#include "ResourceManager.h"

void PlayerSystem::Init()
{
	ResourceManager::LoadTexture("res/img/icon2.png", true, "bullet");
	texture = ResourceManager::GetTexture("bullet");
}

void PlayerSystem::Update(float deltaTime)
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entity);
		auto& movement = ECSHandler::Instance()->GetComponent<Movement>(entity);

		movement.velocity = glm::vec2(0.f);

		if (Core::Instance()->KeyDown(SDL_SCANCODE_A))
		{
			movement.velocity.x = -movement.speed * deltaTime;
		}
		if (Core::Instance()->KeyDown(SDL_SCANCODE_D))
		{
			movement.velocity.x = movement.speed * deltaTime;
		}
		if (Core::Instance()->KeyDown(SDL_SCANCODE_W))
		{
			movement.velocity.y = -movement.speed * deltaTime;
		}
		if (Core::Instance()->KeyDown(SDL_SCANCODE_S))
		{
			movement.velocity.y = movement.speed * deltaTime;
		}

		if (Core::Instance()->GetLMBState())
		{
			ECSEntity bullet = ECSHandler::Instance()->CreateEntity();
			ECSHandler::Instance()->AddComponent(bullet, Transform{ glm::vec2(transform.position.x, transform.position.y), glm::vec2(1.f), 90.f });
			ECSHandler::Instance()->AddComponent(bullet, Rendering{ ResourceManager::GetShader("sprite"), texture });
			ECSHandler::Instance()->AddComponent(bullet, Movement{ 125.f });

			auto& bulletTransform = ECSHandler::Instance()->GetComponent<Transform>(bullet);
			auto& bulletRender = ECSHandler::Instance()->GetComponent<Rendering>(bullet);
			auto& bulletMovement = ECSHandler::Instance()->GetComponent<Movement>(bullet);

			//bullets.push_back(bullet);
			glm::vec2 mousePos = glm::vec2(Core::Instance()->GetMousePosX(), Core::Instance()->GetMousePosY());
			bulletTransform.size = glm::vec2(bulletRender.texture.Width, bulletRender.texture.Height) * bulletTransform.scale;

			float vectorToTarget = glm::distance(transform.position, mousePos);

			glm::vec2 direction = glm::normalize(mousePos - transform.position);
			bulletMovement.velocity = glm::vec2(direction.x * bulletMovement.speed, direction.y * bulletMovement.speed);

			float rotation = std::atan2(direction.y, direction.x) * 180.f / (float)M_PI;
			bulletTransform.rotation = rotation + 90;
		}
	}
}
