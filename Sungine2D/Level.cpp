#include "Level.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include "Transform.h"
#include "Rendering.h"
#include "Collider.h"

#include "ResourceManager.h"
#include "ECSHandler.h"

void Level::Init(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
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

			}
			break;

			case 1:
			{
				texture = ResourceManager::GetTexture("grass");
				ECSEntity tile = ECSHandler::Instance()->CreateEntity();
				ECSHandler::Instance()->AddComponent(tile, Transform{ glm::vec2(j * tileWidth, i * tileHeight) });
				ECSHandler::Instance()->AddComponent(tile, Rendering{ shader, texture });
			}
			break;

			case 2:
			{
				texture = ResourceManager::GetTexture("wall");
				ECSEntity tile = ECSHandler::Instance()->CreateEntity();
				ECSHandler::Instance()->AddComponent(tile, Transform{ glm::vec2(j * tileWidth, i * tileHeight) });
				ECSHandler::Instance()->AddComponent(tile, Rendering{ shader, texture });
				ECSHandler::Instance()->AddComponent(tile, Collider{ });
			}
			break;
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
