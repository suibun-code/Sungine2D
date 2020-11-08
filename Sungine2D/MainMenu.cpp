#include "MainMenu.h"

#include <iostream>
#include <random>

//GLEW
#include "glew.h"

//Misc
#include "Core.h"
#include "GameInstance.h"
#include "ShaderUtil.h"
#include "SuSpriteRenderer.h"
#include "Entity.h"
#include "ResourceManager.h"

//States
#include "TestState.h"

void MainMenu::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	shader = ResourceManager::GetShader("sprite");
	SuTexture2D texture;

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/CornerOfMemories.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	ResourceManager::LoadTexture("res/img/sunginelogo.png", true, "logo");

	texture = ResourceManager::GetTexture("logo");
	ECSEntity logo = ECSHandler::Instance()->CreateEntity();
	ECSHandler::Instance()->GetComponent<EntityData>(logo).name = "Logo";
	ECSHandler::Instance()->AddComponent(logo, TransformComponent{ 1.f, 0.f, glm::vec2((Core::Instance()->GetWindowWidth() / 2) - (texture.Width / 2), (Core::Instance()->GetWindowHeight() / 2) - (texture.Height / 2)), glm::vec2(1.f, 1.f) });
	ECSHandler::Instance()->AddComponent(logo, RenderComponent{ shader, texture, glm::vec3(1.f) });

	ResourceManager::AddText("enter", "Press Enter To Start", glm::vec2(Core::Instance()->GetWindowWidth() / 2, (Core::Instance()->GetWindowHeight() / 2) + 25), ResourceManager::GetFont("CircularMedium"));

	std::cout << ECSHandler::Instance()->ActiveEntityCount() << "\n";

	State::Enter();
}

void MainMenu::Update(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_RETURN))
	{
		Core::Instance()->GetFSM()->ChangeState(new TestState);
		return;
	}

	//Update the texts of all entities that contain a text component.
	Core::Instance()->GetSystem<TextSystem>()->Update();

	State::Update(deltaTime);
}

void MainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw renderable entities.
	Core::Instance()->GetSystem<RenderSystem>()->Draw();

	State::Render();
}

void MainMenu::Exit()
{
	Core::Instance()->GetAM()->ClearMusic();

	//Clear all remaining entities.
	for (auto& entity : mEntities)
	{
		ECSHandler::Instance()->DestroyEntity(entity);
		mEntities.clear();
	}

	//Clear text map.
	ResourceManager::ClearTexts();

	ResourceManager::ClearEntities();

	std::cout << ECSHandler::Instance()->ActiveEntityCount() << "\n";

	State::Exit();
}
