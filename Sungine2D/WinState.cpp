#include "WinState.h"


#include "Core.h"
#include "MainMenu.h"
#include "RenderSystem.h"

void WinState::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.2f, .2f, .2f, 1.f);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/victory.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	ECSEntity entity;

	entity = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->GetComponent<EntityData>(entity).name = "Test";
	ECSHandler::Instance()->GetComponent<EntityData>(entity).tag = "Test";

	ResourceManager::LoadTexture("res/img/win.png", false, "cat");

	ECSHandler::Instance()->AddComponent(entity, Transform{ glm::vec2(0.f, 0.f) });
	ECSHandler::Instance()->AddComponent(entity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("cat"), glm::vec4(1.f, 1.f, 1.f, 1.f) });

	ResourceManager::AddText("winner", "The winner is " + whoWon + "!", ResourceManager::GetFont("CircularMediumLarge"), glm::vec2(Core::Instance()->GetWindowWidth() / 2 - 120.f, (Core::Instance()->GetWindowHeight() / 2) - 150.f), { 255, 0, 180, 255 });
	ResourceManager::AddText("return", "Press 'T' to return to the main menu.", ResourceManager::GetFont("CircularMedium"), glm::vec2(Core::Instance()->GetWindowWidth() / 2 - 120.f, (Core::Instance()->GetWindowHeight() / 2) - 115.f), { 255, 0, 180, 255 });

	Core::Instance()->GetSystem<TextSystem>()->Init();
	Core::Instance()->GetSystem<RenderSystem>()->Init();
	
	State::Enter();
}

void WinState::HandleStateEvents(SDL_Event* event)
{
}

void WinState::Update(float deltaTime)
{
	State::Update(deltaTime);
}

void WinState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Draw renderable entities.
	Core::Instance()->GetSystem<RenderSystem>()->Draw();

	State::Render();
}

void WinState::LateUpdate(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_T))
	{
		Core::Instance()->GetFSM()->ChangeState(new MainMenu());
		return;
	}
}

void WinState::Exit()
{
	State::Exit();
}
