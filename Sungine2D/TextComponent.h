#pragma once

#include "SuFont.h"

struct TextComponent
{
private:
	std::string mInput = "Default";
	const char* mOutput = "Default";
	SuFont font;
};
