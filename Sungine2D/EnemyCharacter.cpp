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
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("char"), glm::vec4(0.f, 0.f, 0.f, 1.f) });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ 650.f, true });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true });

	auto& data = ECSHandler::Instance()->GetComponent<EntityData>(mEntity);

	ECSHandler::Instance()->AddComponent(mEntity, Character{ 1000, ResourceManager::AddText(data.name, "0", ResourceManager::GetFont("CircularBlack"), glm::vec2(0.f), { 0, 0, 175, 255 }) });

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
	auto& movement = ECSHandler::Instance()->GetComponent<Movement>(mEntity);

	ECSHandler::Instance()->GetComponent<Text>(character.healthText).ChangeText(std::to_string(character.health));

	if (transform.IsDirty())
		ECSHandler::Instance()->GetComponent<Transform>(character.healthText).SetPosition(glm::vec2(transform.position.x + 5, transform.position.y - 25));

	movement.velocity = glm::vec2(0.f);

	rendering.color = glm::vec4((float)character.health / 1000, 0.f, 0.f, 1.f);

	if (character.health <= 0)
		Destroy();
}

bool EnemyCharacter::OnCollision(ECSEntity other)
{
	auto& dataOther = ECSHandler::Instance()->GetComponent<EntityData>(other);
	auto& player = ECSHandler::Instance()->GetComponent<Character>(mEntity);

	if (dataOther.tag == "DamageTile")
		player.health -= 1;

	if (dataOther.tag == "Breakable")
	{
		dataOther.script->Destroy();

		auto& movement = ECSHandler::Instance()->GetComponent<Movement>(mEntity);
		movement.acceleration = glm::vec2(0.f);
		
		return true;
	}

	return false;
}
