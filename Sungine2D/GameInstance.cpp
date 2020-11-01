#include "GameInstance.h"

//IMGUI + OPENGL/SDL
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "Core.h"
#include "Entity.h"
#include <iostream>

//Initialzie game instance.
GameInstance* GameInstance::mspGameInstance = nullptr;

GameInstance::GameInstance()
{
	//Update ImGui.	
	if (Core::Instance()->IsRunning())
	{
		if (mDisplayUI)
			UpdateImGui();
	}
}

void GameInstance::UpdateImGui()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(Core::Instance()->GetWindow());
	ImGui::NewFrame();

	if (mUIFont != nullptr)
		ImGui::PushFont(mUIFont);

	std::string windowString = "Game Instance";

	ImGui::SetNextWindowSize(ImVec2(150, 100), 0);

	ImGui::Begin(&windowString[0], NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_MenuBar);

	ImGui::SetWindowPos(ImVec2(0, 0), true);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::Separator();
			ImGui::MenuItem("About", NULL, &mDisplayAbout);
			ImGui::EndMenu();
		}

		ImGui::NextColumn();

		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::Separator();
			ImGui::MenuItem("Log", NULL, &mDisplayLog);
			ImGui::Separator();
			ImGui::MenuItem("Entity List", NULL, &mDisplayEntityList);
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (mDisplayAbout)
	{
		ImGui::Begin("About", &mDisplayAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.55f, 0.235f, 0.75f, 0.95f), "Sungine2D");
		ImGui::Text("Developed by Ramin Amiri");
		ImGui::NewLine();
		ImGui::Text("Developed using:");
		ImGui::Text("- SDL 2.0.12");
		ImGui::Text("- OpenGL 3.3");
		ImGui::Text("- ImGui");
		ImGui::NewLine();
		ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 0.95f), "https://github.com/suibun-code/Sungine2D");

		ImGui::End();
	}

	if (mDisplayEntityList)
	{
		ImGui::Begin("Entity List", &mDisplayEntityList, ImGuiWindowFlags_AlwaysAutoResize);

		s = "Number of active entities: " + std::to_string(Entity::GetGameObjectCount());
		c = s.c_str();
		ImGui::Text(c); 

		ImGui::NewLine();

		for (unsigned int i = 0; i < Entity::GetAllEntities().size(); i++)
		{
			s = Entity::GetAllEntities().at(i)->GetName();
			c = s.c_str();
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 0.95f), c);
		}

		ImGui::End();
	}

	ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
	mLog.Draw("Log", &mDisplayLog);

	ImGui::PopFont();

	ImGui::End();
	ImGui::EndFrame();
}

void GameInstance::Render()
{
	//Render ImGui.
	if (mDisplayUI)
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_SetRenderDrawColor(Core::Instance()->GetRenderer(), 255, 255, 255, 255);
	}
}

void GameInstance::Update(float deltaTime)
{
	//Update ImGui.
	if (mDisplayUI)
		UpdateImGui();
}

void GameInstance::HandleEvents(const SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		if (event->key.keysym.scancode == SDL_SCANCODE_GRAVE)
			if (Core::Instance()->GameInstanceEnabled() == false)
				Core::Instance()->SetGameInstance(true);
			else
				Core::Instance()->SetGameInstance(false);
		break;
	}
}

void GameInstance::DumpStartupLog()
{
	if (!mLogInputBuffer.empty())
	{
		while (!mLogInputBuffer.empty())
		{

			mLog.AddLog("[%.1f sec]: %s", ImGui::GetTime(), mLogInputBuffer.front());
			mLogInputBuffer.erase(mLogInputBuffer.begin());
			//mLogInputBuffer.front() = std::move(mLogInputBuffer.back());
			//mLogInputBuffer.pop_back();
			mLog.AddLog("\n");
		}
	}
	else
		mLog.AddLog("GameInstance::DumpStartupLog() called. Buffer is empty.");
}

void GameInstance::LogBuffer(const char* arr[], int len)
{
	char buffer[256];

	strncpy_s(buffer, arr[0], sizeof(buffer));

	for (int i = 1; i < len; i++)
		strncat_s(buffer, arr[i], sizeof(buffer));

	GameInstance::Instance()->AddLog(buffer);
}

void GameInstance::AddLog(const char* fmt, ...)
{
	if (Core::Instance()->IsRunning())
		mLog.AddLog("[%.1f sec]: %s", ImGui::GetTime(), fmt);
	else
		mLogInputBuffer.push_back(fmt);
}