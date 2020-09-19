#include "Core.h"

#include <iostream>
#include <chrono>
#include <ctime>

//SDL
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

//GLEW
#include "glew.h"

//IMGUI + SDL
#include "imgui_impl_sdl.h"

//IMGUI + OPENGL
#include "imgui_impl_opengl3.h"

#include "GameInstance.h"
#include "ResourceManager.h"
#include "TestState.h"

Core* Core::mspInstance = nullptr;

Core::Core() {}
Core::~Core() {}

bool Core::InitAll(const char* title, const int xpos, const int ypos, const int width, const int height, const int flags)
{
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		GameInstance::Instance()->AddLog("[OK] SDL Core Initialization.");

		//Use OpenGL 3.3 core.
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		//Create SDL window.
		mpSDLWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
	}
	else
	{
		GameInstance::Instance()->AddLog("[FAIL] SDL Core Initialization.");
		return false;
	}

	if (mpSDLWindow != nullptr)
	{
		GameInstance::Instance()->AddLog("[OK] SDL Window Initialization.");
		mpSDLRenderer = SDL_CreateRenderer(mpSDLWindow, -1, SDL_RENDERER_ACCELERATED);
	}
	else
	{
		GameInstance::Instance()->AddLog("[FAIL] SDL Window Initialization.");
		return false;
	}

	if (mpSDLRenderer != nullptr)
	{
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
		GameInstance::Instance()->AddLog("[OK] SDL Renderer Initialization.");
		SDL_SetRenderDrawColor(mpSDLRenderer, 255, 0, 0, 255);
		SDL_RenderSetLogicalSize(mpSDLRenderer, mWindowWidth, mWindowHeight);
		SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
		SDL_SetWindowMinimumSize(mpSDLWindow, 640, 360);
	}
	else
	{
		GameInstance::Instance()->AddLog("[FAIL] SDL Renderer Initialization.");
		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) != 0)
	{
		GameInstance::Instance()->AddLog("[OK] SDL Image Initialization.");

		mpSDLSurface = IMG_Load("res/img/icon.png");
		SDL_SetWindowIcon(mpSDLWindow, mpSDLSurface);
		SDL_FreeSurface(mpSDLSurface);
	}
	else
	{
		GameInstance::Instance()->AddLog("[FAIL] SDL Image Initialization.");
		return false;
	}

	if (Mix_Init(MIX_INIT_MP3) != 0)
	{
		Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);
		Mix_AllocateChannels(16);

		GameInstance::Instance()->AddLog("[OK] SDL Mixer Initialization.");
	}
	else
	{
		GameInstance::Instance()->AddLog("[FAIL] SDL Mixer Initialization.");
		return false;
	}

	if (TTF_Init() == 0)
	{
		GameInstance::Instance()->AddLog("[OK] SDL Font Initialization.");
	}
	else
	{
		GameInstance::Instance()->AddLog("[FAIL] SDL Font Initialization.");
		return false;
	}

	mGLContext = SDL_GL_CreateContext(mpSDLWindow);

	if (mGLContext == NULL)
	{
		printf("OpenGL context failed to be created. SDL Error: %s\n", SDL_GetError());
		mIsRunning = false;
	}
	else
	{
		//Initialize GLEW.
		glewExperimental = true;
		GLenum glewError = glewInit();

		if (glewError != GLEW_OK)
		{
			printf("Error initializing GLEW. %s\n", glewGetErrorString(glewError));
		}
	}

	//OpenGL configuration.
	glViewport(0, 0, mWindowWidth, mWindowHeight);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//ImGui.
	mImGuiContext = ImGui::CreateContext();
	ImGui::SetCurrentContext(mImGuiContext);
	ImGui_ImplSDL2_InitForOpenGL(mpSDLWindow, mGLContext);
	ImGui_ImplOpenGL3_Init("#version 330");

	InitImGui();

	//Set random seed.
	srand((unsigned)time(NULL));

	//Start engine and enable the game instance.
	mIsRunning = true;
	mGameInstanceEnabled = true;

	GameInstance::Instance()->DumpStartupLog();
	GameInstance::Instance()->AddLog("Welcome to Sungine2D.\n");

	mpKeyStates = SDL_GetKeyboardState(nullptr);
	mpFSM = new StateMachine();
	//mpAM = new AudioManager();
	//mpAM->SetMusicVolume(15);
	//mpAM->LoadSound("res/audio/effect/menubtn.wav");
	mpFSM->ChangeState(new TestState());

	return true;
}

void Core::InitImGui()
{
	//*****Input*****

	ImGuiIO& io = ImGui::GetIO();

	GameInstance::Instance()->SetUIFont(io.Fonts->AddFontFromFileTTF("font/CircularStd-Black.ttf", 13.0f));

	//keyboard mapping
	//ImGui will use those indices to peek into the io.KeysDown[] array
	io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
	io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
	io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
	io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
	io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
	io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
	io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
	io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
	io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
	io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
	io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;

	io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
	io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
	io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
	io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
	io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
	io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;

	//*****Style*****

	// purple colors, 3 intensities
	#define HI(v)   ImVec4(0.333f, 0.102f, 0.545f, v)
	#define MED(v)  ImVec4(0.392f, 0.325f, 0.580f, v)
	#define LOW(v)  ImVec4(0.222f, 0.051f, 0.470f, v)
	// backgrounds (@todo: complete with BG_MED, BG_LOW)
	#define BG(v)   ImVec4(0.20f, 0.220f, 0.270f, v)
	// text
	#define TEXTT(v) ImVec4(0.860f, 0.930f, 0.890f, v)

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Text] = TEXTT(0.78f);
	style.Colors[ImGuiCol_TextDisabled] = TEXTT(0.28f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 0.95f); //window bg
	//style.Colors[ImGuiCol_ChildWindowBg] = BG(0.58f);
	style.Colors[ImGuiCol_PopupBg] = BG(0.9f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.31f, 0.31f, 1.0f, 0.0f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
	style.Colors[ImGuiCol_FrameBg] = BG(1.0f);
	style.Colors[ImGuiCol_FrameBgHovered] = MED(0.78f);
	style.Colors[ImGuiCol_FrameBgActive] = MED(1.0f);
	style.Colors[ImGuiCol_TitleBg] = LOW(1.0f);
	style.Colors[ImGuiCol_TitleBgActive] = HI(1.0f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = BG(0.75f);
	style.Colors[ImGuiCol_MenuBarBg] = BG(0.47f);
	style.Colors[ImGuiCol_ScrollbarBg] = BG(1.0f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.0f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = MED(1.0f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.0f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.0f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
	style.Colors[ImGuiCol_ButtonHovered] = MED(0.86f);
	style.Colors[ImGuiCol_ButtonActive] = MED(1.0f);
	style.Colors[ImGuiCol_Header] = MED(0.76f);
	style.Colors[ImGuiCol_HeaderHovered] = MED(0.86f);
	style.Colors[ImGuiCol_HeaderActive] = HI(1.0f);
	//style.Colors[ImGuiCol_Column] = ImVec4(0.14f, 0.16f, 0.19f, 1.0f);
	//style.Colors[ImGuiCol_ColumnHovered] = MED(0.78f);
	//style.Colors[ImGuiCol_ColumnActive] = MED(1.0f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
	style.Colors[ImGuiCol_ResizeGripHovered] = MED(0.78f);
	style.Colors[ImGuiCol_ResizeGripActive] = MED(1.0f);
	style.Colors[ImGuiCol_PlotLines] = TEXTT(0.63f);
	style.Colors[ImGuiCol_PlotLinesHovered] = MED(1.0f);
	style.Colors[ImGuiCol_PlotHistogram] = TEXTT(0.63f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = MED(1.0f);
	style.Colors[ImGuiCol_TextSelectedBg] = MED(0.43f);
	// [...]
	style.Colors[ImGuiCol_ModalWindowDarkening] = BG(0.73f);

	style.WindowPadding = ImVec2(6, 4);
	style.WindowRounding = 0.0f;
	style.FramePadding = ImVec2(5, 2);
	style.FrameRounding = 3.0f;
	style.ItemSpacing = ImVec2(7, 1);
	style.ItemInnerSpacing = ImVec2(1, 1);
	style.TouchExtraPadding = ImVec2(0, 0);
	style.IndentSpacing = 6.0f;
	style.ScrollbarSize = 12.0f;
	style.ScrollbarRounding = 16.0f;
	style.GrabMinSize = 20.0f;
	style.GrabRounding = 2.0f;

	style.WindowTitleAlign.x = 0.5f;

	style.Colors[ImGuiCol_Border] = ImVec4(0.539f, 0.479f, 0.255f, 0.162f);
	style.FrameBorderSize = 0.0f;
	style.WindowBorderSize = 1.0f;
}

bool Core::Tick()
{
	mPreviousTime = mCurrentTime;
	mCurrentTime = SDL_GetPerformanceCounter();
	mDeltaTime = (float)((mCurrentTime - mPreviousTime) * 1000 / (float)SDL_GetPerformanceFrequency());
	mFramesPerSecond = 1000 / mDeltaTime;
	std::cout << "d: " << mDeltaTime << "\n";
	auto duration = std::chrono::steady_clock::now().time_since_epoch();
	auto count = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
	mTick = 1000000 / FPS;
	//if (count % mTick < 325) // Margin of error for modulus.
	//{
	//	if (mGotTick == false) // Drops potential duplicate frames.
	//		mGotTick = true;
	//}
	//else mGotTick = false;
	mGotTick = true;

	return mGotTick;
}

bool Core::KeyDown(SDL_Scancode k)
{
	if (mpKeyStates != nullptr)
	{
		if (mpKeyStates[k] == 1)
			return true;
		else
			return false;
	}
	return false;
}

void Core::WaitFor(int time)
{
	SDL_Delay(time);
}

void Core::Update(float DeltaTime)
{
	mpFSM->Update(mDeltaTime);
}

void Core::Render()
{
	mpFSM->Render();
}

void Core::HandleEvents()
{
	//std::cout << "H: " << mGameInstanceEnabled << "\n";

	ImGuiIO& io = ImGui::GetIO();

	if (SDL_PollEvent(&mEvent))
	{
		ImGui_ImplSDL2_ProcessEvent(&mEvent); //does most of the input/event processing

		if (mEvent.type == SDL_WINDOWEVENT)
		{
			switch (mEvent.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				GameInstance::Instance()->AddLog("Window resized.\n");
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				//if (!mpAM->is_paused())
				//{
				//	mMusicEnabled = false;
				//	mpAM->pause_music();
				//}
				//else
				//	mMusicEnabled = true;
				break;

			case SDL_WINDOWEVENT_FOCUS_GAINED:
				//if (mpAM->IsPaused() && mMusicEnabled == false)
				//	mpAM->ResumeMusic();
				break;
			}
		}

		switch (mEvent.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (mEvent.button.button == SDL_BUTTON_LEFT)
				if (mEvent.button.state == SDL_PRESSED)
					mLMBState = true;
			break;

		case SDL_MOUSEBUTTONUP:
			if (mEvent.button.button == SDL_BUTTON_LEFT)
				mLMBState = false;
			break;

		case SDL_MOUSEMOTION:
			mMousePosX = mEvent.motion.x;
			mMousePosY = mEvent.motion.y;
			break;
		}
	}
	mpFSM->HandleStateEvents(&mEvent);
}

void Core::Quit()
{
	mIsRunning = false;
}

void Core::Clean()
{
	//Clean-up ResourceManager.
	ResourceManager::Clear();

	//Clean-up ImGui.
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	//deallocate program
	//shaderUtil.delete_shaders();

	//Clean-up SDL.
	SDL_DestroyRenderer(mpSDLRenderer);
	SDL_GL_DeleteContext(mGLContext);
	SDL_DestroyWindow(mpSDLWindow);
	mpSDLWindow = NULL;
	mpSDLRenderer = NULL;
	IMG_Quit();
	TTF_Quit();
	SDL_Quit();
}

int Core::Round64(int num)
{
	int multiple = 64;
	int remainder = num % multiple;

	if (num < multiple)
		return 0;
	if (multiple == 0)
		return num;
	if (remainder == 0)
		return num;

	return num - remainder;
}
