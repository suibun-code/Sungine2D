#pragma once

#include "SuFont.h"
#include "ResourceManager.h"

struct TextComponent
{
	std::string output = "Default";
	SuFont font = ResourceManager::GetFont("CircularMedium");
	SDL_Color color = { 0, 0, 0, 255 };
	SuTexture2D fontTexture = SuTexture2D();
};
