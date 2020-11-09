#pragma once

#include "SuFont.h"
#include "ResourceManager.h"

struct TextComponent
{
	std::string output = "Default";
	SuFont font = ResourceManager::GetFont("CircularMedium");
	SuTexture2D fontTexture = SuTexture2D();
};
