#pragma once

#include "SDL_ttf.h"

struct SuFont
{
	const char* mPath = "font/CircularStd-Black.ttf";
	int mSize = 14;
	SDL_Color mTextColor = { 0, 0, 0, 255 };
};