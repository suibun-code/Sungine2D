#include "GameInstance.h"

//IMGUI + OPENGL/SDL
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

#include "Core.h"
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

		if (ImGui::BeginMenu("Log"))
		{
			ImGui::Separator();
			ImGui::MenuItem("Show", NULL, &mDisplayLog);
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	if (mDisplayAbout)
	{
		ImGui::Begin("About", &mDisplayAbout, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();
		ImGui::TextColored(ImVec4(0.5f, 0.f, 0.5f, 1.f), "Sungine2D");
		ImGui::Text("Developed by Ramin Amiri");
		ImGui::NewLine();
		ImGui::Text("Developed using:");
		ImGui::Text("- SDL 2.0.12");
		ImGui::Text("- OpenGL 3.3");
		ImGui::Text("- ImGui");
		ImGui::NewLine();
		ImGui::Text("https://github.com/suibun-code/Sungine2D");
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

void GameInstance::AddLog(const char* fmt, ...)
{
	if (Core::Instance()->IsRunning())
		mLog.AddLog("[%.1f sec]: %s", ImGui::GetTime(), fmt);
	else
		mLogInputBuffer.push_back(fmt);
}