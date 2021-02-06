#include "EnemyCharacter.h"

#include "Collider.h"
#include "ECSHandler.h"
#include "Character.h"
#include "Core.h"
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
	mEntity = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).name = "Enemy";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Enemy";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).script = this;


	ECSHandler::Instance()->AddComponent(mEntity, Transform{ });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("char"), glm::vec3(0.f, 0.f, 0.f) });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ 20000.f, true });

	auto& data = ECSHandler::Instance()->GetComponent<EntityData>(mEntity);

	ECSHandler::Instance()->AddComponent(mEntity, Character{ 1000, ResourceManager::AddText(data.name, "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 0, 175, 175, 255 }) });

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
	auto& rendering = ECSHandler::Instance()->GetComponent<Rendering>(mEntity);

	ECSHandler::Instance()->GetComponent<Text>(character.healthText).ChangeText(std::to_string(character.health));

	if (transform.IsDirty())
		ECSHandler::Instance()->GetComponent<Transform>(character.healthText).SetPosition(glm::vec2(transform.position.x + 5, transform.position.y - 25));

	rendering.color = glm::vec3((float)character.health / 1000, 0.f, 0.f);

	if (character.health <= 0)
		Destroy();
}

bool EnemyCharacter::OnCollision(ECSEntity other)
{
	if (ECSHandler::Instance()->GetComponent<EntityData>(other).tag == "DamageTile")
		ECSHandler::Instance()->GetComponent<Character>(mEntity).health -= 1;

	return false;
}
