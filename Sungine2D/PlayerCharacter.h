#pragma once

#include "SuTexture2D.h"
#include "EntityData.h"
#include "ECSHandler.h"

#include "Bullet.h"

class PlayerCharacter : public BehaviourScript
{
private:
	ECSEntity mEntity;
	std::vector<Bullet*> mBulletOP;

public:
	static SuTexture2D mTexture;

	PlayerCharacter();

	void Start() override;
	void Destroy() override;
	void Update() override;

	ECSEntity GetEntity();
	static void OnCollision(ECSEntity other);
};
