#include "PlayerCharacter.h"

#include "ResourceManager.h"

#include "Transform.h"
#include "Rendering.h"
#include "Movement.h"
#include "Collider.h"
#include "Player.h"

SuTexture2D PlayerCharacter::mTexture;

PlayerCharacter::PlayerCharacter()
{
	BehaviourScript::Generate();
}

void PlayerCharacter::Start()
{
	mTexture = mTexture = ResourceManager::GetTexture("char");

	mEntity = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).name = "Player";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Player";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).script = this;
	
	ECSHandler::Instance()->AddComponent(mEntity, Transform{ glm::vec2(1200.f, 600.f) });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), mTexture });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ });
	ECSHandler::Instance()->AddComponent(mEntity, Player{ });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true });

	//ECSHandler::Instance()->GetComponent<Collider>(mEntity).OnCollision = OnCollision;

	//for (int i = 0; i < 20; i++)
	//{
	//	Bullet* bullet = new Bullet();
	//	mBulletOP.push_back(bullet);
	//}
	
	std::cout << "Made player\n";
}

void PlayerCharacter::Destroy()
{
}

void PlayerCharacter::Update()
{
}

ECSEntity PlayerCharacter::GetEntity()
{
	return mEntity;
}

bool PlayerCharacter::OnCollision(ECSEntity other)
{
	return false;
}
