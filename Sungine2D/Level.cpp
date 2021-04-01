#include "Level.h"

#include <fstream>
#include <sstream>

#include "Transform.h"
#include "Rendering.h"
#include "Collider.h"

#include "ResourceManager.h"
#include "ECSHandler.h"
#include "EntityData.h"
#include "Tile.h"

void Level::Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	ShaderUtil shader = ResourceManager::GetShader("sprite");
	SuTexture2D texture;

	ResourceManager::LoadTexture("res/img/grass1.png", true, "grass");
	ResourceManager::LoadTexture("res/img/wall.png", true, "wall");
	ResourceManager::LoadTexture("res/img/lava.png", true, "lava");
	ResourceManager::LoadTexture("res/img/wall_breakable.png", true, "wall_breakable");

	for (unsigned int i = 0; i < levelHeight; i++)
	{
		for (unsigned int j = 0; j < levelWidth; j++)
		{
			if (tileData[i][j] > 0)
			{
				switch (tileData[i][j])
				{
				case 1:
				{
					Tile* tile = new Tile();
					tile->SetOwner(tile);
					ECSHandler::Instance()->GetComponent<Transform>(tile->GetEntity()).position = glm::vec2(j * tileWidth, i * tileHeight);
					ECSHandler::Instance()->GetComponent<Rendering>(tile->GetEntity()).texture = ResourceManager::GetTexture("grass");
				}
				break;

				case 2:
				{
					Tile* tile = new Tile();
					tile->SetOwner(tile);
					ECSHandler::Instance()->GetComponent<Transform>(tile->GetEntity()).position = glm::vec2(j * tileWidth, i * tileHeight);
					ECSHandler::Instance()->GetComponent<Rendering>(tile->GetEntity()).texture = ResourceManager::GetTexture("wall");
					ECSHandler::Instance()->AddComponent(tile->GetEntity(), Collider{ });
				}
				break;
				
				case 3:
				{
					Tile* tile = new Tile();
					tile->SetOwner(tile);
					ECSHandler::Instance()->GetComponent<EntityData>(tile->GetEntity()).tag = "DamageTile";
					ECSHandler::Instance()->GetComponent<Transform>(tile->GetEntity()).position = glm::vec2(j * tileWidth, i * tileHeight);
					ECSHandler::Instance()->GetComponent<Rendering>(tile->GetEntity()).texture = ResourceManager::GetTexture("lava");
					ECSHandler::Instance()->AddComponent(tile->GetEntity(), Collider{ false, false, true });
				}
				break;

				case 4:
				{
					Tile* tile = new Tile();
					tile->SetOwner(tile);
					ECSHandler::Instance()->GetComponent<EntityData>(tile->GetEntity()).tag = "Breakable";
					ECSHandler::Instance()->GetComponent<Rendering>(tile->GetEntity()).texture = ResourceManager::GetTexture("wall_breakable");
					ECSHandler::Instance()->GetComponent<Transform>(tile->GetEntity()).position = glm::vec2(j * tileWidth, i * tileHeight);
					ECSHandler::Instance()->AddComponent(tile->GetEntity(), Collider{ false, false, true });
				}

				}
			}
		}
	}
}

void Level::Load(const char* file)
{
	int tileCode;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<unsigned int>> tileData;

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
			Init(tileData, levelWidth, levelHeight);
	}
}

void Level::Draw()
{

}
