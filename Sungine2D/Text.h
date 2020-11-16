#pragma once

#include "SuFont.h"
#include "ResourceManager.h"

struct Text
{
	std::string output = "Default";
	SuFont font = ResourceManager::GetFont("CircularMedium");
	SDL_Color color = { 0, 0, 0, 255 };
	SuTexture2D fontTexture = SuTexture2D();
	bool dirty = false;

	void ChangeText(std::string output)
	{
		if (this->output != output)
		{
			this->output = output;
			dirty = true;
		}
	}
};
