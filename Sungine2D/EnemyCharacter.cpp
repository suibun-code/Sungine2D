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
	
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Character";
	ECSHandler::Instance()->AddComponent(mEntity, Transform{ });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("char"), glm::vec3(.75f, .5f, .5f) });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true });
	ECSHandler::Instance()->AddComponent(mEntity, Character{ });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ 20000.f });
	
	auto& data = ECSHandler::Instance()->GetComponent<EntityData>(mEntity);

	healthText = ResourceManager::AddText(data.name, "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 0, 0, 175, 255 });;
	ECSHandler::Instance()->GetComponent<Text>(healthText).ChangeText(std::to_string(health));
}

void EnemyCharacter::Destroy()
{
	BehaviourScript::Destroy();
}

void EnemyCharacter::Update(float deltaTime)
{
	auto& transform = ECSHandler::Instance()->GetComponent<Transform>(mEntity);
	auto& character = ECSHandler::Instance()->GetComponent<Character>(mEntity);
	auto& movement = ECSHandler::Instance()->GetComponent<Movement>(mEntity);

	if (character.health == 50)
		movement.speed = 30000.f;

	else if (character.health == 25)
		movement.speed = 40000.f;

	else if (character.health <= 0)
		Destroy();

	ECSHandler::Instance()->GetComponent<Text>(character.healthText).ChangeText(std::to_string(character.health));

	if (transform.IsDirty())
		ECSHandler::Instance()->GetComponent<Transform>(character.healthText).SetPosition(glm::vec2(transform.position.x + 5, transform.position.y - 25));
}

bool EnemyCharacter::OnCollision(ECSEntity other)
{
	return false;
}
