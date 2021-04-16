#include "PauseState.h"

#include "Core.h"
#include "MainMenu.h"

void PauseState::Enter()
{
	ECSEntity entity;

	entity = ECSHandler::Instance()->CreateEntity();

	ECSHandler::Instance()->GetComponent<EntityData>(entity).name = "Test";
	ECSHandler::Instance()->GetComponent<EntityData>(entity).tag = "Test";

	ResourceManager::LoadTexture("res/img/pausemenu.png", true, "cat");

	ECSHandler::Instance()->AddComponent(entity, Transform{ glm::vec2(0.f, 0.f) });
	ECSHandler::Instance()->AddComponent(entity, Rendering{ ResourceManager::GetShader("sprite"), ResourceManager::GetTexture("cat"), glm::vec4(1.f, 1.f, 1.f, 1.f) });

	Core::Instance()->GetSystem<RenderSystem>()->Init();

	std::cout << "name: " << ECSHandler::Instance()->GetComponent<EntityData>(mEntities.back()).name << "\n";
	
	State::Enter();
}

void PauseState::HandleStateEvents(SDL_Event* event)
{
}

void PauseState::Update(float deltaTime)
{
	//Core::Instance()->GetFSM()->GetStates().front()->Update(deltaTime);
}

void PauseState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	Core::Instance()->GetFSM()->GetStates().front()->Render();

	//Draw renderable entities.
	Core::Instance()->GetSystem<RenderSystem>()->Draw();
	
	State::Render();
}

void PauseState::LateUpdate(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_ESCAPE))
	{
		Core::Instance()->GetFSM()->PopState();
		return;
	}
	
	if (Core::Instance()->KeyDown(SDL_SCANCODE_T))
	{
		Core::Instance()->GetFSM()->ChangeState(new MainMenu());
		return;
	}
}

void PauseState::Exit()
{
	std::cout << "exit pause\n";

	//Destroy all active entities.
	for (int i = mEntities.size() - 1; i >= 0; i--)
		ECSHandler::Instance()->DestroyEntity(mEntities.at(i));

	std::cout << "FINAL ENTITIES: " << ECSHandler::Instance()->ActiveEntityCount() << "\n";
	std::cout << "RenderSystem entity count: " << Core::Instance()->GetSystem<RenderSystem>()->mEntities.size() << "\n";
	
	const char* msg[] = { "[EXIT] '", mStateName, "'.\n" };
	GameInstance::LogBuffer(msg, sizeof(msg) / sizeof(msg[0]));
}
