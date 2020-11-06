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

ECSEntity text;
ECSEntity logo;

void MainMenu::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	shader = ResourceManager::GetShader("sprite");
	SuTexture2D texture;

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/CornerOfMemories.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	ResourceManager::LoadFont("font/CircularStd-Medium.ttf", 14, { 0, 0, 0, 255 }, "CircularMedium");
	ResourceManager::LoadTexture("res/img/sunginelogo.png", true, "logo");
	ResourceManager::LoadTexture("res/img/grasstile1.png", true, "grass");

	texture = ResourceManager::GetTexture("logo");
	logo = ECSHandler::Instance()->CreateEntity();
	ECSHandler::Instance()->AddComponent(logo, TransformComponent{ 1.f, 0.f, glm::vec2((Core::Instance()->GetWindowWidth() / 2) - (texture.Width / 2), (Core::Instance()->GetWindowHeight() / 2) - (texture.Height / 2)), glm::vec2(1.f, 1.f) });
	ECSHandler::Instance()->AddComponent(logo, RenderComponent{ shader, texture, glm::vec3(1.f) });

	text = ECSHandler::Instance()->CreateEntity();
	ECSHandler::Instance()->AddComponent(text, TransformComponent{ 1.f, 0.f, glm::vec2(Core::Instance()->GetWindowWidth() / 2, (Core::Instance()->GetWindowHeight() / 2) + 25) });
	ECSHandler::Instance()->AddComponent(text, RenderComponent{ shader });
	ECSHandler::Instance()->AddComponent(text, TextComponent{ "Press Enter To Start", ResourceManager::GetFont("CircularMedium") });
	ECSHandler::Instance()->GetComponent<TextComponent>(text).font.textColor = SDL_Color({ 190, 112, 128, 255 });

	State::Enter();
}

void MainMenu::Update(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_RETURN))
	{
		Core::Instance()->GetFSM()->ChangeState(new TestState);
		return;
	}

	Core::Instance()->GetSystem<TextSystem>()->Update();
	std::cout << "TEXTSYSTEM: " << Core::Instance()->GetSystem<TextSystem>()->mEntities.size() << "\n";

	State::Update(deltaTime);
}

void MainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw renderable entities.
	Core::Instance()->GetSystem<RenderSystem>()->Draw();
	std::cout << "RENDERSYSTEM: " << Core::Instance()->GetSystem<RenderSystem>()->mEntities.size() << "\n";

	State::Render();
}

void MainMenu::Exit()
{
	Core::Instance()->GetAM()->ClearMusic();

	ECSHandler::Instance()->DestroyEntity(logo);
	ECSHandler::Instance()->DestroyEntity(text);

	std::cout << "TEXTSYSTEM FINAL: " << Core::Instance()->GetSystem<TextSystem>()->mEntities.size() << "\n";
	std::cout << "RENDERSYSTEM FINAL: " << Core::Instance()->GetSystem<RenderSystem>()->mEntities.size() << "\n";

	//Destroy text entities and all other entities.
	ResourceManager::ClearTexts();
	ResourceManager::ClearEntities();

	State::Exit();
}
