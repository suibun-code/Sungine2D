#include "Bullet.h"

#include "ResourceManager.h"

#include "Transform.h"
#include "Rendering.h"
#include "Movement.h"
#include "Collider.h"

int Bullet::mBulletCount = 0;
SuTexture2D Bullet::mTexture;

Bullet::Bullet()
{
	if (mBulletCount == 0)
	{
		ResourceManager::LoadTexture("res/img/bullet3.png", true, "bullet");
		mTexture = ResourceManager::GetTexture("bullet");
	}

	mEntity = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->GetComponent<EntityData>(mEntity).tag = "Bullet";

	ECSHandler::Instance()->AddComponent(mEntity, Transform{  });
	ECSHandler::Instance()->AddComponent(mEntity, Rendering{ ResourceManager::GetShader("sprite"), mTexture });
	ECSHandler::Instance()->AddComponent(mEntity, Movement{ 450.f });
	ECSHandler::Instance()->AddComponent(mEntity, Collider{ true, true });

	ECSHandler::Instance()->GetComponent<Collider>(mEntity).OnCollision = OnCollision;

	mBulletCount++;
}

Bullet::~Bullet()
{
	mBulletCount--;
}

ECSEntity Bullet::GetEntity()
{
	return mEntity;
}

void Bullet::OnCollision()
{
	//std::cout << "Bullet\n";
}