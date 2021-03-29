#include "FollowSystem.h"

#include "ECSHandler.h"

#include "Transform.h"
#include "Movement.h"
#include "Follow.h"

void FollowSystem::Init()
{
}

void FollowSystem::Update(float deltaTime)
{
	for (auto const& entity : mEntities)
	{
		if (ECSHandler::Instance()->GetComponent<Follow>(entity).target == NULL)
			continue;

		auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entity);
		auto& movement = ECSHandler::Instance()->GetComponent<Movement>(entity);
		auto& target = ECSHandler::Instance()->GetComponent<Transform>(ECSHandler::Instance()->GetComponent<Follow>(entity).target);

		if (transform.position.x < target.position.x)
		{
			movement.acceleration.x += movement.speed * deltaTime;
			transform.dirty = true;
		}
		else if (transform.position.x > target.position.x)
		{
			movement.acceleration.x += -movement.speed * deltaTime;
			transform.dirty = true;
		}
		if (transform.position.y < target.position.y)
		{
			movement.acceleration.y += movement.speed * deltaTime;
			transform.dirty = true;
		}
		else if (transform.position.y > target.position.y)
		{
			movement.acceleration.y += -movement.speed * deltaTime;
			transform.dirty = true;
		}
	}
}
