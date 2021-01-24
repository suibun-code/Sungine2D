#pragma once

#include "SuTexture2D.h"
#include "EntityData.h"
#include "ECSHandler.h"

class Bullet
{
private:
	ECSEntity mEntity;

public:
	static int mBulletCount;
	static SuTexture2D mTexture;

	Bullet();
	~Bullet();

	ECSEntity GetEntity();
	static void OnCollision();
};
