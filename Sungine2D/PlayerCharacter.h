#pragma once

#include "SuTexture2D.h"
#include "EntityData.h"
#include "ECSHandler.h"

#include "Bullet.h"

class PlayerCharacter
{
private:
	ECSEntity mEntity;
	std::vector<Bullet*> mBulletOP;

public:
	static SuTexture2D mTexture;

	PlayerCharacter();

	ECSEntity GetEntity();
	static void OnCollision();
};
