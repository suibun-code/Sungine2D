#include "MappingState.h"

#include "Core.h"
#include "ResourceManager.h"

//STATES
#include "MainMenu.h"

void MappingState::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.294f, .0f, .509f, 1.f);

	shader = ResourceManager::GetShader("sprite");

	ResourceManager::LoadTexture("res/img/wall.png", false, "wall");
	ResourceManager::LoadTexture("res/img/grass.png", false, "grass");

	for (unsigned int i = 0; i < 12; i++)
	{
		for (unsigned int i = 0; i < 20; i++)
		{
			row.push_back(0);
		}
		tileData.push_back(row);
	}

	for (unsigned int i = 0; i < 12; i++)
	{
		for (unsigned int j = 0; j < 20; j++)
		{
			switch (tileData[i][j])
			{
			case 0:
			{
				std::cout << "Made empty.\n";
			}
			break;

			case 1:
			{
				texture = ResourceManager::GetTexture("grass");
				ECSEntity tile = ECSHandler::Instance()->CreateEntity();
				ECSHandler::Instance()->AddComponent(tile, Transform{ glm::vec2(j * 64, i * 64) });
				ECSHandler::Instance()->AddComponent(tile, Rendering{ shader, texture });
				entityData[i][j] = tile;
			}
			break;

			case 2:
			{
				texture = ResourceManager::GetTexture("wall");
				ECSEntity tile = ECSHandler::Instance()->CreateEntity();
				ECSHandler::Instance()->AddComponent(tile, Transform{ glm::vec2(j * 64, i * 64) });
				ECSHandler::Instance()->AddComponent(tile, Rendering{ shader, texture });
				ECSHandler::Instance()->AddComponent(tile, Collider{ });
				entityData[i][j] = tile;
				
			}
			break;
			}
		}
	}

	Core::Instance()->GetSystem<RenderSystem>()->Init();

	State::Enter();
}

void MappingState::Update(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_T))
	{
		Core::Instance()->GetFSM()->ChangeState(new MainMenu());
		return;
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_0))
	{
		currentTile = 0;
		std::cout << "Switched to 0.\n";
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_1))
	{
		currentTile = 1;
		std::cout << "Switched to 1.\n";
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_2))
	{
		currentTile = 2;
		std::cout << "Switched to 2.\n";
	}

	if (Core::Instance()->GetLMBState())
	{
		//std::cout << "mouseposX: " << Core::Instance()->Round64(Core::Instance()->GetMousePosX()) << "\n";
		//std::cout << "mouseposY: " << Core::Instance()->Round64(Core::Instance()->GetMousePosY()) << "\n\n";

		int posX = Core::Instance()->Round64(Core::Instance()->GetMousePosX());
		int posY = Core::Instance()->Round64(Core::Instance()->GetMousePosY());

		//ECSEntity tile;

		switch (currentTile)
		{
		case 0:
		{

		}
		break;
		case 1:
		{
			texture = ResourceManager::GetTexture("grass");

			if (entityData[posY / 64][posX / 64] == NULL)
			{
				entityData[posY / 64][posX / 64] = ECSHandler::Instance()->CreateEntity();
				ECSHandler::Instance()->AddComponent(entityData[posY / 64][posX / 64], Transform{ glm::vec2(posX, posY) });
				ECSHandler::Instance()->AddComponent(entityData[posY / 64][posX / 64], Rendering{ shader, texture });
			}
			else
			{
				auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[posY / 64][posX / 64]).texture = texture;
			}
		}
		break;
		case 2:
		{
			texture = ResourceManager::GetTexture("wall");

			if (entityData[posY / 64][posX / 64] == NULL)
			{
				entityData[posY / 64][posX / 64] = ECSHandler::Instance()->CreateEntity();
				ECSHandler::Instance()->AddComponent(entityData[posY / 64][posX / 64], Transform{ glm::vec2(posX, posY) });
				ECSHandler::Instance()->AddComponent(entityData[posY / 64][posX / 64], Rendering{ shader, texture });
			}
			else
			{
				auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[posY / 64][posX / 64]).texture = texture;
			}
		}
		break;
		default:
		{

		}
		break;
		}

		tileData[posY / 64][posX / 64] = currentTile;

		std::cout << "Mapped to: X " << posX / 64 << " Y " << posY / 64 << "\n";;

		auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entityData[posY / 64][posX / 64]);
		auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[posY / 64][posX / 64]);

		transform.size = glm::vec2(render.texture.Width, render.texture.Height) * transform.scale;
	}

	State::Update(deltaTime);
}

void MappingState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Core::Instance()->GetSystem<RenderSystem>()->Draw();

	State::Render();
}

void MappingState::Exit()
{
	State::Exit();
}
