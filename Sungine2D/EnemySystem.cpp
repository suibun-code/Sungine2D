#include "EnemySystem.h"

#include "ECSHandler.h"
#include "ResourceManager.h"

#include "Transform.h"
#include "Text.h"
#include "Enemy.h"
#include "EntityData.h"

void EnemySystem::Init()
{
	for (auto const& entity : mEntities)
	{
		auto& enemy = ECSHandler::Instance()->GetComponent<Enemy>(entity);
		auto& data = ECSHandler::Instance()->GetComponent<EntityData>(entity);

		enemy.healthText = ResourceManager::AddText(data.name, "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 0, 0, 175, 255 });;
	}
}

void EnemySystem::Update(float deltaTime)
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entity);
		auto& enemy = ECSHandler::Instance()->GetComponent<Enemy>(entity);

		//ECSHandler::Instance()->GetComponent<Text>(ResourceManager::Texts["EnemyHP"]).output = "100";
		ECSHandler::Instance()->GetComponent<Text>(enemy.healthText).ChangeText(std::to_string(enemy.health));
		ECSHandler::Instance()->GetComponent<Transform>(enemy.healthText).position = glm::vec2(transform.position.x + 5, transform.position.y - 25);
	}
}
