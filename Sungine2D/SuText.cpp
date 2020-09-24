#include "SuText.h"

SuText::SuText()
{
	Update(0, glm::vec2(1.f), SuFont());
}

SuText::SuText(std::string input, glm::vec2 pos, SuFont font)
{
	UpdateS(input, pos, font);
}

void SuText::UpdateVars(glm::vec2 pos, SuFont font)
{
	mTexture = ResourceManager::LoadTextureFromFont(mOutput, true, font);
	mTransform.position = pos;
	mTransform.size = glm::vec2(mTexture.Width, mTexture.Height) * mTransform.scale;
}

void SuText::UpdateS(std::string input, glm::vec2 pos, SuFont font)
{
	mOutput = input.c_str();

	UpdateVars(pos, font);
}

void SuText::DrawS(SuSprite& renderer, std::string input, glm::vec2 pos, SuFont font)
{
	UpdateS(input, pos, font);

	Entity::Draw(renderer);
}

void SuText::DrawS(SuSprite& renderer)
{
	Entity::Draw(renderer);
}
