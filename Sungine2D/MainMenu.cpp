#include "MainMenu.h"

#include <memory>

//GLEW
#include "glew.h"

//Misc
#include "Core.h"
#include "GameInstance.h"
#include "ShaderUtil.h"
#include "SuSpriteRenderer.h"
#include "Entity.h"
#include "ResourceManager.h"

//Components
#include "TransformComponent.h"
#include "RenderComponent.h"

//Systems
#include "MovementSystem.h"
#include "RenderSystem.h"

//ECSHandler
#include "ECSHandler.h"

//States
#include "TestState.h"

Entity* logo;

ECSHandler handler;
std::shared_ptr<MovementSystem> movementSystem;
std::shared_ptr<RenderSystem> renderSystem;

void MainMenu::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	shader = ResourceManager::GetShader("sprite");
	SuTexture2D texture;
	ECSEntity logo;
	Signature signature;

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/CornerOfMemories.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	ResourceManager::LoadFont("font/CircularStd-Medium.ttf", 14, { 0, 0, 0, 255 }, "CircularMedium");
	ResourceManager::LoadTexture("res/img/sunginelogo.png", true, "logo");
	ResourceManager::LoadTexture("res/img/grasstile1.png", true, "grass");

	ResourceManager::AddText("LogoText", "Press Enter To Start", glm::vec2(Core::Instance()->GetWindowWidth() / 2, (Core::Instance()->GetWindowHeight() / 2) + 25), ResourceManager::GetFont("CircularMedium"));

	handler.Init();
	handler.RegisterComponent<TransformComponent>();
	handler.RegisterComponent<RenderComponent>();

	movementSystem = handler.RegisterSystem<MovementSystem>();
	renderSystem = handler.RegisterSystem<RenderSystem>();

	signature.set(handler.GetComponentType<TransformComponent>());
	signature.set(handler.GetComponentType<RenderComponent>());
	handler.SetSystemSignature<MovementSystem>(signature);
	handler.SetSystemSignature<RenderSystem>(signature);

	renderSystem->Init();

	texture = ResourceManager::GetTexture("logo");

	logo = handler.CreateEntity();
	handler.AddComponent(logo, TransformComponent{ 1.f, 0.f, glm::vec2((Core::Instance()->GetWindowWidth() / 2) - (texture.Width / 2), (Core::Instance()->GetWindowHeight() / 2) - (texture.Height / 2)), glm::vec2(1.f, 1.f) });
	handler.AddComponent(logo, RenderComponent{ shader, texture, glm::vec3(1.f) });

	State::Enter();
}

void MainMenu::Update(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_RETURN))
		Core::Instance()->GetFSM()->ChangeState(new TestState);

	State::Update(deltaTime);
}

void MainMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw renderable entities.
	renderSystem->Draw();

	//Draw texts.
	for (std::map<std::string, SuText*>::iterator it = ResourceManager::Texts.begin(); it != ResourceManager::Texts.end(); it++)
		it->second->Draw(*renderer);

	State::Render();
}

void MainMenu::Exit()
{
	Core::Instance()->GetAM()->ClearMusic();

	//Destroy text entities and all other entities.
	ResourceManager::ClearTexts();
	ResourceManager::ClearEntities();

	State::Exit();
}
