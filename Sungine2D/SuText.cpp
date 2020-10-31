#include "SuText.h"

#include "ResourceManager.h"

using namespace Sungine;

SuText::SuText()
{
	Update(glm::vec2(1.f), SuFont());
}

SuText::SuText(std::string input, glm::vec2 pos, SuFont font)
{
	mInput = input;
	Update(pos, font);
}

void SuText::UpdateVars(glm::vec2 pos, SuFont font)
{
	mTexture = ResourceManager::LoadTextureFromFont(mOutput, true, font);
	mTransform.position = pos;
	mTransform.size = glm::vec2(mTexture.Width, mTexture.Height) * mTransform.scale;
}

void SuText::Update(glm::vec2 pos, SuFont font)
{
	mOutput = mInput.c_str();
	UpdateVars(pos, font);
}

void SuText::Draw(SuSpriteRenderer& renderer)
{
	Entity::Draw(renderer);
}

void SuText::Draw(SuSpriteRenderer& renderer, std::string input, glm::vec2 pos, SuFont font)
{
	mInput = input;
	Update(pos, font);

	Entity::Draw(renderer);
}

void SuText::ChangeText(std::string input)
{
	mInput = input;
}
