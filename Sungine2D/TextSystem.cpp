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

		ResourceManager::LoadTextureFromFont(&render.texture, text.output, true, text.font, text.color);
	}
}

void TextSystem::Update()
{
	for (auto const& entity : mEntities)
	{
		auto& transform = ECSHandler::Instance()->GetComponent<TransformComponent>(entity);
		auto& render = ECSHandler::Instance()->GetComponent<RenderComponent>(entity);
        auto& text = ECSHandler::Instance()->GetComponent<TextComponent>(entity);

        ResourceManager::LoadTextureFromFont(&text.fontTexture, text.output, true, text.font, text.color);
		render.texture = text.fontTexture;

		transform.size = glm::vec2(render.texture.Width, render.texture.Height) * transform.scale;
	}
}
