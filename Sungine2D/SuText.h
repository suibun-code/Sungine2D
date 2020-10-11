#pragma once

#include <typeinfo>

#include "Entity.h"
#include "SuFont.h"
#include "ResourceManager.h"

namespace Sungine
{
	class SuText : public Entity
	{
	private:
		std::string mInput = "Default";
		const char* mOutput = "Default";

	public:
		SuText(std::string input, glm::vec2 pos, SuFont font);
		SuText();

		void UpdateVars(glm::vec2 pos, SuFont font);

		void Update(std::string input, glm::vec2 pos, SuFont font);
		void Draw(SuSprite& renderer);
		void Draw(SuSprite& renderer, std::string input, glm::vec2 pos, SuFont font);
	};
}

