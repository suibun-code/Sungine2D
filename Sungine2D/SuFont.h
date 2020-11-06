#pragma once

#include "SDL_ttf.h"

struct SuFont
{
	const char* path = "font/CircularStd-Black.ttf";
	int size = 14;
	SDL_Color textColor = SDL_Color{ 0, 0, 0, 255 };
};