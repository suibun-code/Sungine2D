#include "EnemySystem.h"

#include "ECSHandler.h"
#include "ResourceManager.h"

#include "Transform.h"
#include "Text.h"
#include "Character.h"
#include "Movement.h"
#include "EntityData.h"

void EnemySystem::Init()
{
	//for (auto const& entity : mEntities)
	//{
	//	auto& enemy = ECSHandler::Instance()->GetComponent<Character>(entity);
	//	auto& data = ECSHandler::Instance()->GetComponent<EntityData>(entity);
	//	auto& movement = ECSHandler::Instance()->GetComponent<Movement>(entity);

	//	enemy.healthText = ResourceManager::AddText(data.name, "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 0, 0, 175, 255 });
	//	ECSHandler::Instance()->GetComponent<Text>(enemy.healthText).ChangeText(std::to_string(enemy.health));

	//	movement.speed = 20000.f;
	//}
}

void EnemySystem::Update(float deltaTime)
{
	//for (auto const& entity : mEntities)
	//{
	//	auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entity);
	//	auto& enemy = ECSHandler::Instance()->GetComponent<Character>(entity);
	//	auto& movement = ECSHandler::Instance()->GetComponent<Movement>(entity);

	//	if (enemy.health == 50)
	//		movement.speed = 30000.f;

	//	if (enemy.health == 25)
	//		movement.speed = 40000.f;

	//	if (transform.position.x < glm::vec2(0, 0).x)
	//	{
	//		movement.velocity.x = movement.speed * deltaTime;
	//		transform.dirty = true;
	//	}
	//	else if (transform.position.x > glm::vec2(0, 0).x)
	//	{
	//		movement.velocity.x = -movement.speed * deltaTime;
	//		transform.dirty = true;
	//	}
	//	if (transform.position.y < glm::vec2(0, 0).y)
	//	{
	//		movement.velocity.y = movement.speed * deltaTime;
	//		transform.dirty = true;
	//	}
	//	else if (transform.position.y > glm::vec2(0, 0).y)
	//	{
	//		movement.velocity.y = -movement.speed * deltaTime;
	//		transform.dirty = true;
	//	}

	//	ECSHandler::Instance()->GetComponent<Text>(enemy.healthText).ChangeText(std::to_string(enemy.health));

	//	if (transform.IsDirty())
	//		ECSHandler::Instance()->GetComponent<Transform>(enemy.healthText).SetPosition(glm::vec2(transform.position.x + 5, transform.position.y - 25));
	//}
}
