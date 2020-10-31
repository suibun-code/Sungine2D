#pragma once

#include <typeinfo>

#include "Entity.h"
#include "SuFont.h"

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
		void Update(glm::vec2 pos, SuFont font);
		void Draw(SuSpriteRenderer& renderer);
		void Draw(SuSpriteRenderer& renderer, std::string input, glm::vec2 pos, SuFont font);
		void ChangeText(std::string input);
	};
}

