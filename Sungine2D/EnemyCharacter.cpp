#include "EnemyCharacter.h"

#include "Collider.h"
#include "ECSHandler.h"
#include "Character.h"
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
	
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).name = "Enemy";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Enemy";
	ECSHandler::Instance()->AddComponent(mEntity, Transform{ });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("char"), glm::vec3(1.f, 0.f, 0.f) });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ 20000.f });
	
	auto& data = ECSHandler::Instance()->GetComponent<EntityData>(mEntity);
	
	ECSHandler::Instance()->AddComponent(mEntity, Character{ 100, ResourceManager::AddText(data.name, "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 0, 0, 175, 255 }) });

	auto& character = ECSHandler::Instance()->GetComponent<Character>(mEntity);
	ECSHandler::Instance()->GetComponent<Text>(character.healthText).ChangeText(std::to_string(character.health));
}

void EnemyCharacter::Destroy()
{
	ResourceManager::ClearText(ECSHandler::Instance()->GetComponent<EntityData>(mEntity).name);
	
	BehaviourScript::Destroy();
}

void EnemyCharacter::Update(float deltaTime)
{
	auto& transform = ECSHandler::Instance()->GetComponent<Transform>(mEntity);
	auto& character = ECSHandler::Instance()->GetComponent<Character>(mEntity);
	auto& movement = ECSHandler::Instance()->GetComponent<Movement>(mEntity);

	ECSHandler::Instance()->GetComponent<Text>(character.healthText).ChangeText(std::to_string(character.health));

	if (transform.IsDirty())
		ECSHandler::Instance()->GetComponent<Transform>(character.healthText).SetPosition(glm::vec2(transform.position.x + 5, transform.position.y - 25));

	if (character.health == 50)
		movement.speed = 30000.f;

	else if (character.health == 25)
		movement.speed = 40000.f;

	else if (character.health <= 0)
		Destroy();
}

bool EnemyCharacter::OnCollision(ECSEntity other)
{
	return false;
}
