#include "MappingState.h"

#include <fstream>
#include <sstream>

//IMGUI + OPENGL/SDL
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "Core.h"
#include "ResourceManager.h"
#include "Level.h"

//STATES
#include "MainMenu.h"

void MappingState::UpdateImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(Core::Instance()->GetWindow());
	ImGui::NewFrame();

	ImGui::SetNextWindowSize(ImVec2(1280, 128), 0);
	ImGui::SetWindowPos(ImVec2(0, 0), true);

	if (mDisplayMapper)
	{
		ImGui::Begin("Tile Mapper", &mDisplayMapper, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_HorizontalScrollbar);

		ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 0.95f), "To begin drawing: select a tile, collapse this window (top left), and hold left click over the canvas. Open this window again to select a new tile (or the eraser). Press 'S' to save, and 'L' to load.");
		ImGui::Separator();
		ImGui::NewLine();

		for (unsigned int i = 0; i < mTextures.size(); i++)
		{
			if (ImGui::ImageButton((void*)(intptr_t)ResourceManager::GetTexture(mTextures.at(i)).ID, ImVec2(64, 64)))
				currentTile = i;

			if (i == 14)
				ImGui::NewLine();
			else
				ImGui::SameLine();
		}

		if (ImGui::IsWindowCollapsed())
			mCollapsed = true;
		else
			mCollapsed = false;

		ImGui::End();
	}

	ImGui::EndFrame();
}

void MappingState::InitLevel(std::vector<std::vector<unsigned int>> tileData, unsigned int levelWidth, unsigned int levelHeight)
{
	ShaderUtil shader = ResourceManager::GetShader("sprite");
	SuTexture2D texture;

	for (unsigned int i = 0; i < levelHeight; i++)
	{
		for (unsigned int j = 0; j < levelWidth; j++)
		{
			if (tileData[i][j] == 0)
			{
				if (entityData[i][j] != NULL)
				{
					ECSHandler::Instance()->DestroyEntity(entityData[i][j]);
					entityData[i][j] = NULL;
				}
			}
			else
			{
				texture = ResourceManager::GetTexture(mTextures.at(tileData[i][j]));

				//std::cout << tileData[i][j] << "\n";

				if (entityData[i][j] != NULL)
				{
					ECSHandler::Instance()->DestroyEntity(entityData[i][j]);
					entityData[i][j] = NULL;
				}

				entityData[i][j] = ECSHandler::Instance()->CreateEntity();
				ECSHandler::Instance()->AddComponent(entityData[i][j], Transform{ glm::vec2(j * 64, i * 64) });
				ECSHandler::Instance()->AddComponent(entityData[i][j], Rendering{ shader, texture });

				auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entityData[i][j]);
				auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[i][j]);

				transform.size = glm::vec2(render.texture.Width, render.texture.Height) * transform.scale;
			}
		}
	}
}

void MappingState::LoadLevel(const char* file)
{
	unsigned int levelWidth = 0;
	unsigned int levelHeight = 0;
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

	ResourceManager::LoadTexture("res/img/destroy.png", true, "destroy");
	ResourceManager::LoadTexture("res/img/grass1.png", true, "grass");
	ResourceManager::LoadTexture("res/img/wall.png", true, "wall");
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
	ResourceManager::LoadTexture("res/img/lava.png", true, "lava");

	for (unsigned int i = 0; i < 12; i++)
	{
		for (unsigned int i = 0; i < 20; i++)
		{
			row.push_back(0);
		}
		tileData.push_back(row);
	}

	Core::Instance()->GetSystem<RenderSystem>()->Init();

	mTextures.push_back("destroy");
	mTextures.push_back("grass");
	mTextures.push_back("wall");
	mTextures.push_back("mud");
	mTextures.push_back("mudleft");
	mTextures.push_back("mudright");
	mTextures.push_back("mud1");
	mTextures.push_back("mud2");
	mTextures.push_back("mud3");
	mTextures.push_back("mud4");
	mTextures.push_back("mud5");
	mTextures.push_back("mud6");
	mTextures.push_back("water");
	mTextures.push_back("waterleft");
	mTextures.push_back("waterright");
	mTextures.push_back("water1");
	mTextures.push_back("water2");
	mTextures.push_back("water3");
	mTextures.push_back("water4");
	mTextures.push_back("water5");
	mTextures.push_back("water6");
	mTextures.push_back("watercornerTL");
	mTextures.push_back("watercornerTR");
	mTextures.push_back("watercornerBL");
	mTextures.push_back("watercornerBR");
	mTextures.push_back("lava");

	State::Enter();
}

void MappingState::HandleStateEvents(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_MIDDLE)
			if (event->button.state == SDL_PRESSED)
				mMMBState = true;
		break;

	case SDL_MOUSEBUTTONUP:
		if (event->button.button == SDL_BUTTON_MIDDLE)
			mMMBState = false;
		break;
	}

	if (mMMBState)
	{
		Core::Instance()->MoveView(glm::vec3(Core::Instance()->GetMouseRelX(), Core::Instance()->GetMouseRelY(), 0));
	}

	if (Core::Instance()->GetLMBDown())
	{
		//std::cout << "xrel:" << Core::Instance()->GetMouseRelX() << "\n";
		//std::cout << "yrel:" << Core::Instance()->GetMouseRelY() << "\n";
		//Core::Instance()->MoveView(glm::vec3(Core::Instance()->GetMouseRelX(), Core::Instance()->GetMouseRelY(), 0));
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
		std::cout << "Loaded.\n";
	}

	if (Core::Instance()->GetLMBDown() && mCollapsed == true)
	{
		int posX = Core::Instance()->Round64(Core::Instance()->GetMousePosX());
		int posY = Core::Instance()->Round64(Core::Instance()->GetMousePosY());
		int x = posX / 64;
		int y = posY / 64;

		if (currentTile == 0)
		{
			if (entityData[y][x] != NULL)
			{
				ECSHandler::Instance()->DestroyEntity(entityData[y][x]);
				entityData[y][x] = NULL;

				tileData[y][x] = currentTile;

				//std::cout << "Destroyed entity\n";
			}
		}
		else
		{
			texture = ResourceManager::GetTexture(mTextures.at(currentTile));
		}

		if (currentTile != 0)
		{
			if (entityData[y][x] == NULL)
			{
				//std::cout << "Created entity\n";
				entityData[y][x] = ECSHandler::Instance()->CreateEntity();
				ECSHandler::Instance()->AddComponent(entityData[y][x], Transform{ glm::vec2(posX, posY) });
				ECSHandler::Instance()->AddComponent(entityData[y][x], Rendering{ shader, texture });
			}
			else
			{
				auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[y][x]).texture = texture;
			}

			tileData[y][x] = currentTile;

			auto& transform = ECSHandler::Instance()->GetComponent<Transform>(entityData[y][x]);
			auto& render = ECSHandler::Instance()->GetComponent<Rendering>(entityData[y][x]);

			transform.size = glm::vec2(render.texture.Width, render.texture.Height) * transform.scale;
		}
	}
}

void MappingState::Update(float deltaTime)
{
	UpdateImGui();

	//State::Update(deltaTime);
}

void MappingState::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	Core::Instance()->GetSystem<RenderSystem>()->Draw();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_GL_SwapWindow(Core::Instance()->GetWindow());
	//State::Render();
}

void MappingState::LateUpdate(float deltaTime)
{
	if (Core::Instance()->KeyDown(SDL_SCANCODE_T))
	{
		Core::Instance()->GetFSM()->ChangeState(new MainMenu());
		return;
	}
}

void MappingState::Exit()
{
	State::Exit();
}
