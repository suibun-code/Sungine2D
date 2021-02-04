#include "Bullet.h"

#include "ResourceManager.h"

#include "Transform.h"
#include "Rendering.h"
#include "Movement.h"
#include "Collider.h"
#include "Character.h"

int Bullet::mBulletCount = 0;
SuTexture2D Bullet::mTexture;

Bullet::Bullet()
{
	BehaviourScript::Generate();
}

Bullet::~Bullet()
{
	
}

void Bullet::Start()  
{
	if (mBulletCount == 0)
	{
		ResourceManager::LoadTexture("res/img/bullet3.png", true, "bullet");
		mTexture = ResourceManager::GetTexture("bullet");
	}

	mEntity = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Bullet";
	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).script = this;

	ECSHandler::Instance()->AddComponent(mEntity, Transform{  });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), mTexture });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ 450.f });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true, true });
	//ECSHandler::Instance()->DisableComponent<Rendering>(mEntity);

	mBulletCount++;
}

void Bullet::Destroy()
{
	mBulletCount--;
	BehaviourScript::Destroy();
}

void Bullet::Update(float deltaTime)
{
	
}

bool Bullet::OnCollision(ECSEntity other)
{
	if (ECSHandler::Instance()->GetComponent<EntityData>(other).tag == "Enemy")
	{
		auto& enemyOther = ECSHandler::Instance()->GetComponent<Character>(other);
		enemyOther.health -= 25;
		ECSHandler::Instance()->DisableComponent<Rendering>(mEntity);
		ECSHandler::Instance()->DisableComponent<Collider>(mEntity);
		return true;
	}
	else if (ECSHandler::Instance()->GetComponent<EntityData>(other).tag == "Player")
	{
		return false;
	}
	else
	{
		//Destroy();
		return false;
	}
}


