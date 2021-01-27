#include "Level.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Transform.h"
#include "Rendering.h"
#include "Collider.h"

#include "ResourceManager.h"
#include "ECSHandler.h"
#include "EntityData.h"

void Level::Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	ShaderUtil shader = ResourceManager::GetShader("sprite");
	SuTexture2D texture;

	ResourceManager::LoadTexture("res/img/wall.png", false, "wall");
	ResourceManager::LoadTexture("res/img/grass.png", false, "grass");
	ResourceManager::LoadTexture("res/img/mud/mud.png", true, "mud");
	ResourceManager::LoadTexture("res/img/mud/mudleft.png", true, "mudleft");
	ResourceManager::LoadTexture("res/img/mud/mudright.png", true, "mudright");
	ResourceManager::LoadTexture("res/img/mud/mud1.png", true, "mud1");
	ResourceManager::LoadTexture("res/img/mud/mud2.png", true, "mud2");
	ResourceManager::LoadTexture("res/img/mud/mud3.png", true, "mud3");
	ResourceManager::LoadTexture("res/img/mud/mud4.png", true, "mud4");
	ResourceManager::LoadTexture("res/img/mud/mud5.png", true, "mud5");
	ResourceManager::LoadTexture("res/img/mud/mud6.png", true, "mud6");
	ResourceManager::LoadTexture("res/img/water/water.png", true, "water");
	ResourceManager::LoadTexture("res/img/water/waterleft.png", true, "waterleft");
	ResourceManager::LoadTexture("res/img/water/waterright.png", true, "waterright");
	ResourceManager::LoadTexture("res/img/water/water1.png", true, "water1");
	ResourceManager::LoadTexture("res/img/water/water2.png", true, "water2");
	ResourceManager::LoadTexture("res/img/water/water3.png", true, "water3");
	ResourceManager::LoadTexture("res/img/water/water4.png", true, "water4");
	ResourceManager::LoadTexture("res/img/water/water5.png", true, "water5");
	ResourceManager::LoadTexture("res/img/water/water6.png", true, "water6");
	ResourceManager::LoadTexture("res/img/water/watercornerTL.png", true, "watercornerTL");
	ResourceManager::LoadTexture("res/img/water/watercornerTR.png", true, "watercornerTR");
	ResourceManager::LoadTexture("res/img/water/watercornerBL.png", true, "watercornerBL");
	ResourceManager::LoadTexture("res/img/water/watercornerBR.png", true, "watercornerBR");

	for (unsigned int i = 0; i < levelHeight; i++)
	{
		for (unsigned int j = 0; j < levelWidth; j++)
		{
			if (tileData[i][j] > 0)
			{
				ECSEntity tile = ECSHandler::Instance()->CreateEntity();

				switch (tileData[i][j])
				{
				case 1:
				{
					texture = ResourceManager::GetTexture("grass");
				}
				break;

				case 2:
				{
					texture = ResourceManager::GetTexture("wall");
					ECSHandler::Instance()->GetComponent<EntityData>(tile).tag = "wall";
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 3:
				{
					texture = ResourceManager::GetTexture("mud");
				}
				break;

				case 4:
				{
					texture = ResourceManager::GetTexture("mudleft");
				}
				break;

				case 5:
				{
					texture = ResourceManager::GetTexture("mudright");
				}
				break;

				case 6:
				{
					texture = ResourceManager::GetTexture("mud1");
				}
				break;

				case 7:
				{
					texture = ResourceManager::GetTexture("mud2");
				}
				break;

				case 8:
				{
					texture = ResourceManager::GetTexture("mud3");
				}
				break;

				case 9:
				{
					texture = ResourceManager::GetTexture("mud4");
				}
				break;

				case 10:
				{
					texture = ResourceManager::GetTexture("mud5");
				}
				break;

				case 11:
				{
					texture = ResourceManager::GetTexture("mud6");
				}
				break;

				case 12:
				{
					texture = ResourceManager::GetTexture("water");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 13:
				{
					texture = ResourceManager::GetTexture("waterleft");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 14:
				{
					texture = ResourceManager::GetTexture("waterright");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 15:
				{
					texture = ResourceManager::GetTexture("water1");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 16:
				{
					texture = ResourceManager::GetTexture("water2");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 17:
				{
					texture = ResourceManager::GetTexture("water3");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 18:
				{
					texture = ResourceManager::GetTexture("water4");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 19:
				{
					texture = ResourceManager::GetTexture("water5");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 20:
				{
					texture = ResourceManager::GetTexture("water6");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 21:
				{
					texture = ResourceManager::GetTexture("watercornerTL");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 22:
				{
					texture = ResourceManager::GetTexture("watercornerTR");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 23:
				{
					texture = ResourceManager::GetTexture("watercornerBL");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				case 24:
				{
					texture = ResourceManager::GetTexture("watercornerBR");
					ECSHandler::Instance()->AddComponent(tile, Collider{ });
				}
				break;

				}

				ECSHandler::Instance()->AddComponent(tile, Transform{ glm::vec2(j * tileWidth, i * tileHeight) });
				ECSHandler::Instance()->AddComponent(tile, Rendering{ shader, texture });
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
