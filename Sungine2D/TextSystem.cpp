#include "TextSystem.h"

//GLM
#include "glm/glm/glm.hpp"

#include "ResourceManager.h"

#include "RenderComponent.h"
#include "TransformComponent.h"
#include "TextComponent.h"

#include <iostream>

void TextSystem::Init()
{
	for (auto const& entity : mEntities)
	{
		auto& render = ECSHandler::Instance()->GetComponent<RenderComponent>(entity);
		auto& text = ECSHandler::Instance()->GetComponent<TextComponent>(entity);

		render.texture = ResourceManager::LoadTextureFromFont(text.output, true, text.font);
	}
}

void TextSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		auto& render = ECSHandler::Instance()->GetComponent<RenderComponent>(entity);
        auto& text = ECSHandler::Instance()->GetComponent<TextComponent>(entity);

        render.texture = ResourceManager::LoadTextureFromFont(text.output, true, text.font);
	}
}
