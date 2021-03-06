#include "MainMenu.h"

//GLEW
#include "glew.h"

//Misc
#include "Core.h"
#include "GameInstance.h"
#include "ResourceManager.h"

//States
#include "TestState.h"
#include "MappingState.h"
#include "TwoPlayerMode.h"
#include "WinState.h"

void MainMenu::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/CornerOfMemories.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

	shader = ResourceManager::GetShader("sprite");
	SuTexture2D texture;

	ResourceManager::LoadTexture("res/img/sunginelogo.png", true, "logo");

	texture = ResourceManager::GetTexture("logo");
	ECSEntity logo = ECSHandler::Instance()->CreateEntity();
	ECSHandler::Instance()->GetComponent<EntityData>(logo).name = "Logo";
	ECSHandler::Instance()->AddComponent(logo, Transform{ glm::vec2((Core::Instance()->GetWindowWidth() / 2) - (texture.Width / 2), (Core::Instance()->GetWindowHeight() / 2) - (texture.Height / 2)) });
	ECSHandler::Instance()->AddComponent(logo, Rendering{ shader, texture });

	ResourceManager::AddText("choose", "Choose the mode you wish to play.", ResourceManager::GetFont("CircularMedium"), glm::vec2(Core::Instance()->GetWindowWidth() / 2 - 120.f, (Core::Instance()->GetWindowHeight() / 2) + 50), { 175, 175, 175, 255 });
	
	ResourceManager::AddText("singleplayer", "'1': Singleplayer Against AI", ResourceManager::GetFont("CircularMedium"), glm::vec2(Core::Instance()->GetWindowWidth() / 2 - 100.f, (Core::Instance()->GetWindowHeight() / 2) + 75), { 0, 165, 100, 255 });
	ResourceManager::AddText("twoplayer", "'2': 1 vs 1 Two-Player", ResourceManager::GetFont("CircularMedium"), glm::vec2(Core::Instance()->GetWindowWidth() / 2 - 100.f, (Core::Instance()->GetWindowHeight() / 2) + 100), { 200, 0, 200, 255 });
	ResourceManager::AddText("mapping", "'M': Create A Map!", ResourceManager::GetFont("CircularMedium"), glm::vec2(Core::Instance()->GetWindowWidth() / 2 - 100.f, (Core::Instance()->GetWindowHeight() / 2) + 125), { 175, 175, 175, 255 });

	Core::Instance()->GetSystem<TextSystem>()->Init();
	Core::Instance()->GetSystem<RenderSystem>()->Init();

	State::Enter();
}

void MainMenu::HandleStateEvents(SDL_Event* event)
{

}

void MainMenu::Update(float deltaTime)
{
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

void MainMenu::LateUpdate(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_1))
	{
		Core::Instance()->GetFSM()->ChangeState(new TestState());
		return;
	}

	if (Core::Instance()->KeyDown(SDL_SCANCODE_2))
	{
		Core::Instance()->GetFSM()->ChangeState(new TwoPlayerMode());
		return;
	}

	if (Core::Instance()->KeyDown(SDL_SCANCODE_M))
	{
		Core::Instance()->GetFSM()->ChangeState(new MappingState());
		return;
	}

	if (Core::Instance()->KeyDown(SDL_SCANCODE_H))
	{
		Core::Instance()->GetFSM()->ChangeState(new WinState("yo"));
		return;
	}
}

void MainMenu::Exit()
{
	State::Exit();

	//std::cout << "FINAL ENTITIES: " << ECSHandler::Instance()->ActiveEntityCount() << "\n";
	//std::cout << "RenderSystem entity count: " << Core::Instance()->GetSystem<RenderSystem>()->mEntities.size() << "\n";
}
