#include "MappingState.h"

#include <fstream>
#include <sstream>

#include "Core.h"
#include "ResourceManager.h"
#include "Level.h"

//STATES
#include "MainMenu.h"

void MappingState::InitLevel(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	ShaderUtil shader = ResourceManager::GetShader("sprite");
	SuTexture2D texture;

	ResourceManager::LoadTexture("res/img/wall.png", false, "wall");
	ResourceManager::LoadTexture("res/img/grass.png", false, "grass");

	for (unsigned int i = 0; i < levelHeight; i++)
	{
		for (unsigned int j = 0; j < levelWidth; j++)
		{
			switch (tileData[i][j])
			{
			case 0:
			{
				if (entityData[i][j] != NULL)
				{
					ECSHandler::Instance()->DestroyEntity(entityData[i][j]);
					entityData[i][j] = NULL;
				}
			}
			break;

			case 1:
			{
				texture = ResourceManager::GetTexture("grass");

				if (entityData[i][j] != NULL)
				{
					ECSHandler::Instance()->DestroyEntity(entityData[i][j]);
					entityData[i][j] = NULL;

					entityData[i][j] = ECSHandler::Instance()->CreateEntity();
					ECSHandler::Instance()->AddComponent(entityData[i][j], Transform{ glm::vec2(j * 64, i * 64) });
					ECSHandler::Instance()->AddComponent(entityData[i][j], Rendering{ shader, texture });

					auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entityData[i][j]);
					auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[i][j]);

					transform.size = glm::vec2(render.texture.Width, render.texture.Height) * transform.scale;
				}
				else
				{
					entityData[i][j] = ECSHandler::Instance()->CreateEntity();
					ECSHandler::Instance()->AddComponent(entityData[i][j], Transform{ glm::vec2(j * 64, i * 64) });
					ECSHandler::Instance()->AddComponent(entityData[i][j], Rendering{ shader, texture });

					auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entityData[i][j]);
					auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[i][j]);

					transform.size = glm::vec2(render.texture.Width, render.texture.Height) * transform.scale;
				}

			}
			break;

			case 2:
			{
				texture = ResourceManager::GetTexture("wall");

				if (entityData[i][j] != NULL)
				{
					ECSHandler::Instance()->DestroyEntity(entityData[i][j]);
					entityData[i][j] = NULL;

					entityData[i][j] = ECSHandler::Instance()->CreateEntity();
					ECSHandler::Instance()->AddComponent(entityData[i][j], Transform{ glm::vec2(j * 64, i * 64) });
					ECSHandler::Instance()->AddComponent(entityData[i][j], Rendering{ shader, texture });
					ECSHandler::Instance()->AddComponent(entityData[i][j], Collider{ });

					auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entityData[i][j]);
					auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[i][j]);

					transform.size = glm::vec2(render.texture.Width, render.texture.Height) * transform.scale;
				}
				else
				{
					entityData[i][j] = ECSHandler::Instance()->CreateEntity();
					ECSHandler::Instance()->AddComponent(entityData[i][j], Transform{ glm::vec2(j * 64, i * 64) });
					ECSHandler::Instance()->AddComponent(entityData[i][j], Rendering{ shader, texture });
					ECSHandler::Instance()->AddComponent(entityData[i][j], Collider{ });

					auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entityData[i][j]);
					auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[i][j]);

					transform.size = glm::vec2(render.texture.Width, render.texture.Height) * transform.scale;
				}
			}
			break;

			default:
			{
				std::cout << "DEFAULT\n";
			}
			break;
			}
			//std::cout << "j: " << j << "\n";
		}
		//std::cout << "i: " << i << "\n";
	}
}

void MappingState::LoadLevel(const char* file)
{
	int levelWidth = 0;
	int levelHeight = 0;
	int tileCode;
	std::string line;
	std::ifstream fstream(file);

	tileData.clear();

	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			if (entityData[i][j] != NULL)
				ECSHandler::Instance()->DestroyEntity(entityData[i][j]);

			entityData[i][j] = NULL;
		}
	}

	if (fstream)
	{
		while (std::getline(fstream, line))
		{
			std::istringstream sstream(line);
			std::vector<unsigned int> row;

			//Read the line of data. Set levelWidth to the longest row.
			while (sstream >> tileCode)
			{
				row.push_back(tileCode);

				if (levelWidth < row.size())
					levelWidth = row.size();
			}

			tileData.push_back(row);
			levelHeight = tileData.size();
		}

		//std::cout << "Level width: " << levelWidth << "\n";
		//std::cout << "Level height: " << levelHeight << "\n";

		if (tileData.size() > 0)
			InitLevel(tileData, 20, 12);
	}
}

void MappingState::Enter()
{
	//Clear the screen with specific color.
	glClearColor(.0f, .0f, .0f, 1.f);

	Core::Instance()->GetAM()->LoadMusic("res/audio/music/midnightcloset.mp3");
	Core::Instance()->GetAM()->PlayMusic(0, -1);

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
	if (Core::Instance()->KeyDown(SDL_SCANCODE_S))
	{
		std::ofstream file;
		file.open("res/levels/saved.txt");

		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < 20; j++)
			{
				if (j == 19)
				{
					file << std::to_string(tileData[i][j]);
					std::cout << "Wrote: " << std::to_string(tileData[i][j]) << "\n";
					continue;
				}
				file << std::to_string(tileData[i][j]) + " ";
				std::cout << "Wrote: " << std::to_string(tileData[i][j]) + " " << "\n";

			}

			if (i == 11)
				continue;

			file << "\n";
		}

		file.close();

		std::cout << "Saved.\n";
	}
	if (Core::Instance()->KeyDown(SDL_SCANCODE_L))
	{
		LoadLevel("res/levels/saved.txt");
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

	if (Core::Instance()->GetLMBDown())
	{
		//std::cout << "mouseposX: " << Core::Instance()->Round64(Core::Instance()->GetMousePosX()) << "\n";
		//std::cout << "mouseposY: " << Core::Instance()->Round64(Core::Instance()->GetMousePosY()) << "\n\n";

		int posX = Core::Instance()->Round64(Core::Instance()->GetMousePosX());
		int posY = Core::Instance()->Round64(Core::Instance()->GetMousePosY());

		switch (currentTile)
		{
		case 0:
		{
			if (entityData[posY / 64][posX / 64] != NULL)
			{
				ECSHandler::Instance()->DestroyEntity(entityData[posY / 64][posX / 64]);
				entityData[posY / 64][posX / 64] = NULL;

				tileData[posY / 64][posX / 64] = currentTile;

				std::cout << "Destroyed entity\n";
			}
		}
		break;
		case 1:
		{
			texture = ResourceManager::GetTexture("grass");
		}
		break;
		case 2:
		{
			texture = ResourceManager::GetTexture("wall");
		}
		break;
		default:
		{

		}
		break;
		}

		if (currentTile != 0)
		{
			if (entityData[posY / 64][posX / 64] == NULL)
			{
				std::cout << "Created entity\n";
				entityData[posY / 64][posX / 64] = ECSHandler::Instance()->CreateEntity();
				ECSHandler::Instance()->AddComponent(entityData[posY / 64][posX / 64], Transform{ glm::vec2(posX, posY) });
				ECSHandler::Instance()->AddComponent(entityData[posY / 64][posX / 64], Rendering{ shader, texture });
			}
			else
			{
				auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[posY / 64][posX / 64]).texture = texture;
			}

			tileData[posY / 64][posX / 64] = currentTile;

			//std::cout << "Mapped to: X " << posX / 64 << " Y " << posY / 64 << "\n";;

			auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entityData[posY / 64][posX / 64]);
			auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[posY / 64][posX / 64]);

			transform.size = glm::vec2(render.texture.Width, render.texture.Height) * transform.scale;
		}
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
