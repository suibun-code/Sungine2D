#pragma once

#include <typeinfo>

#include "Entity.h"
#include "SuFont.h"
#include "ResourceManager.h"

#include <iostream> //DELETETETETTETETETE

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

		template <class T>
		void Update(T input, glm::vec2 pos, SuFont font)
		{
			mInput = std::to_string(input);
			mOutput = mInput.c_str();

			UpdateVars(pos, font);
		}

		void UpdateS(std::string input, glm::vec2 pos, SuFont font);

		template <class T>
		void Draw(SuSprite& renderer, T input, glm::vec2 pos, SuFont font)
		{
			Update(input, pos, font);

			Entity::Draw(renderer);
		}
		template <class T>
		void Draw(SuSprite& renderer)
		{
			Entity::Draw(renderer);
		}

		void DrawS(SuSprite& renderer, std::string input, glm::vec2 pos, SuFont font);
		void DrawS(SuSprite& renderer);
	};
}

