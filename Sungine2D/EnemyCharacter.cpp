#include "EnemyCharacter.h"

#include "Collider.h"
#include "ECSHandler.h"
#include "Enemy.h"
#include "EntityData.h"
#include "Movement.h"
#include "Rendering.h"
#include "Text.h"
#include "Transform.h"

EnemyCharacter::EnemyCharacter()
{
	BehaviourScript::Generate();
}

void EnemyCharacter::Start()
{
	//spawn at: spawnLocations[i]

	mEntity = ECSHandler::Instance()->CreateEntity();
	
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Enemy";
	ECSHandler::Instance()->AddComponent(mEntity, Transform{ });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("char"), glm::vec3(.75f, .5f, .5f) });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true });
	ECSHandler::Instance()->AddComponent(mEntity, Enemy{ });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ 20000.f });
	
	auto& data = ECSHandler::Instance()->GetComponent<EntityData>(mEntity);

	healthText = ResourceManager::AddText(data.name, "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 0, 0, 175, 255 });;
	ECSHandler::Instance()->GetComponent<Text>(healthText).ChangeText(std::to_string(health));
}

void EnemyCharacter::Destroy()
{
}

void EnemyCharacter::Update(float deltaTime)
{
	auto& transform = ECSHandler::Instance()->GetComponent<Transform>(mEntity);
	auto& enemy = ECSHandler::Instance()->GetComponent<Enemy>(mEntity);
	auto& movement = ECSHandler::Instance()->GetComponent<Movement>(mEntity);

	if (enemy.health == 50)
		movement.speed = 30000.f;

	if (enemy.health == 25)
		movement.speed = 40000.f;

	ECSHandler::Instance()->GetComponent<Text>(enemy.healthText).ChangeText(std::to_string(enemy.health));

	if (transform.IsDirty())
		ECSHandler::Instance()->GetComponent<Transform>(enemy.healthText).SetPosition(glm::vec2(transform.position.x + 5, transform.position.y - 25));
}

bool EnemyCharacter::OnCollision(ECSEntity other)
{
	return false;
}
