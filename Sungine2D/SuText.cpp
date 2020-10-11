#include "SuText.h"

SuText::SuText()
{
	Update("Default", glm::vec2(1.f), SuFont());
}

SuText::SuText(std::string input, glm::vec2 pos, SuFont font)
{
	Update(input, pos, font);
}

void SuText::UpdateVars(glm::vec2 pos, SuFont font)
{
	mTexture = ResourceManager::LoadTextureFromFont(mOutput, true, font);
	mTransform.position = pos;
	mTransform.size = glm::vec2(mTexture.Width, mTexture.Height) * mTransform.scale;
}

void SuText::Update(std::string input, glm::vec2 pos, SuFont font)
{
	mOutput = input.c_str();

	UpdateVars(pos, font);
}

void SuText::Draw(SuSprite& renderer, std::string input, glm::vec2 pos, SuFont font)
{
	Update(input, pos, font);

	Entity::Draw(renderer);
}

void SuText::Draw(SuSprite& renderer)
{
	Entity::Draw(renderer);
}
