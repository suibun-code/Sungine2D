#include "Core.h"

#include <iostream>
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
#include "ECSHandler.h"

#include "MainMenu.h"
#include "TestState.h"

typedef std::chrono::high_resolution_clock Clock;

//Initialzie singleton instances.
Core* Core::mspInstance = nullptr;
ECSHandler* ECSHandler::mspHandlerInstance = nullptr;
GameInstance* GameInstance::mspGameInstance = nullptr;

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
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		Mix_AllocateChannels(32);

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

	//Set random seed.
	srand((unsigned)time(NULL));

	//OpenGL configuration.
	glViewport(0, 0, mWindowWidth, mWindowHeight);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_SCISSOR_TEST);

	//ImGui.
	mImGuiContext = ImGui::CreateContext();
	ImGui::SetCurrentContext(mImGuiContext);
	ImGui_ImplSDL2_InitForOpenGL(mpSDLWindow, mGLContext);
	ImGui_ImplOpenGL3_Init("#version 330");

	//Load shaders and attach variables/values to variables within the shader source code.
	ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("shaders/followParticle.vert", "shaders/followParticle.frag", nullptr, "particle");

	glMatrixMode(GL_MODELVIEW);

	mProjection = glm::ortho(0.f, 1280.f, 720.f, 0.f, -5.f, 5.f);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").SetMatrix4("projection", mProjection);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", mProjection);

	mView = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	mView = glm::translate(mView, glm::vec3(0, 0, 0));
	ResourceManager::GetShader("particle").SetMatrix4("view", mView);
	ResourceManager::GetShader("sprite").SetMatrix4("view", mView);

	//Initialize ImGui.
	InitImGui();

	//Dump the items that were added to the GameInstance log before the GameInstance log was instantiated.
	GameInstance::Instance()->DumpStartupLog();
	GameInstance::Instance()->AddLog("Welcome to Sungine2D.\n");

	//Fonts.
	ResourceManager::LoadFont("font/CircularStd-Medium.ttf", 14, "CircularMedium");
	ResourceManager::LoadFont("font/CircularStd-Black.ttf", 14, "CircularBlack");

	ECSHandler::Instance()->Init();

	//Register components.
	ECSHandler::Instance()->RegisterComponent<EntityData>();
	ECSHandler::Instance()->RegisterComponent<Character>();
	ECSHandler::Instance()->RegisterComponent<Transform>();
	ECSHandler::Instance()->RegisterComponent<Movement>();
	ECSHandler::Instance()->RegisterComponent<Rendering>();
	ECSHandler::Instance()->RegisterComponent<Text>();
	ECSHandler::Instance()->RegisterComponent<Collider>();
	ECSHandler::Instance()->RegisterComponent<Follow>();
	ECSHandler::Instance()->RegisterComponent<Particle>();

	//Register systems.
	mpMovementSystem = ECSHandler::Instance()->RegisterSystem<MovementSystem>();
	mpRenderSystem = ECSHandler::Instance()->RegisterSystem<RenderSystem>();
	mpParticleRenderSystem = ECSHandler::Instance()->RegisterSystem<ParticleRenderSystem>();
	mpTextSystem = ECSHandler::Instance()->RegisterSystem<TextSystem>();
	mpCollisionSystem = ECSHandler::Instance()->RegisterSystem<CollisionSystem>();
	mpFollowSystem = ECSHandler::Instance()->RegisterSystem<FollowSystem>();

	Signature movementSignature;
	movementSignature.set(ECSHandler::Instance()->GetComponentType<Movement>());
	movementSignature.set(ECSHandler::Instance()->GetComponentType<Transform>());
	movementSignature.set(ECSHandler::Instance()->GetComponentType<Rendering>());
	ECSHandler::Instance()->SetSystemSignature<MovementSystem>(movementSignature);

	Signature renderSignature;
	renderSignature.set(ECSHandler::Instance()->GetComponentType<Transform>());
	renderSignature.set(ECSHandler::Instance()->GetComponentType<Rendering>());
	ECSHandler::Instance()->SetSystemSignature<RenderSystem>(renderSignature);

	Signature particleRenderSignature;
	particleRenderSignature.set(ECSHandler::Instance()->GetComponentType<Transform>());
	particleRenderSignature.set(ECSHandler::Instance()->GetComponentType<Rendering>());
	particleRenderSignature.set(ECSHandler::Instance()->GetComponentType<Particle>());
	ECSHandler::Instance()->SetSystemSignature<ParticleRenderSystem>(particleRenderSignature);

	Signature textSignature;
	textSignature.set(ECSHandler::Instance()->GetComponentType<Transform>());
	textSignature.set(ECSHandler::Instance()->GetComponentType<Rendering>());
	textSignature.set(ECSHandler::Instance()->GetComponentType<Text>());
	ECSHandler::Instance()->SetSystemSignature<TextSystem>(textSignature);

	Signature collisionSignature;
	collisionSignature.set(ECSHandler::Instance()->GetComponentType<Transform>());
	collisionSignature.set(ECSHandler::Instance()->GetComponentType<Collider>());
	ECSHandler::Instance()->SetSystemSignature<CollisionSystem>(collisionSignature);

	Signature followSignature;
	followSignature.set(ECSHandler::Instance()->GetComponentType<Transform>());
	followSignature.set(ECSHandler::Instance()->GetComponentType<Movement>());
	followSignature.set(ECSHandler::Instance()->GetComponentType<Follow>());
	ECSHandler::Instance()->SetSystemSignature<FollowSystem>(followSignature);

	mpSystems.insert({ typeid(MovementSystem).name(), mpMovementSystem });
	mpSystems.insert({ typeid(RenderSystem).name(), mpRenderSystem });
	mpSystems.insert({ typeid(ParticleRenderSystem).name(), mpParticleRenderSystem });
	mpSystems.insert({ typeid(TextSystem).name(), mpTextSystem });
	mpSystems.insert({ typeid(CollisionSystem).name(), mpCollisionSystem });
	mpSystems.insert({ typeid(FollowSystem).name(), mpFollowSystem });

	mpKeyStates = SDL_GetKeyboardState(nullptr);
	mpFSM = new StateMachine();
	mpAM = new AudioManager();
	mpAM->SetMusicVolume(15);
	mpFSM->ChangeState(new MainMenu());

	//Start engine and enable the game instance.
	mIsRunning = true;
	mGameInstanceEnabled = true;
	
	return true;
}

void Core::InitImGui()
{
	//*****Input*****

	//Global ImGui font.
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("font/CircularStd-Black.ttf", 13.0f);
	//GameInstance::Instance()->SetUIFont(io.Fonts->AddFontFromFileTTF("font/CircularStd-Black.ttf", 13.0f));
	//io.Fonts->Clear();

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

void Core::MoveView(glm::vec3 translate)
{
	mView = glm::translate(mView, translate);
	ResourceManager::GetShader("sprite").SetMatrix4("view", mView);
}

bool Core::Tick()
{
	mCurrentTime = SDL_GetTicks();
	mDeltaTime = (float)(mCurrentTime - mPreviousTime) / 1000;
	mPreviousTime = mCurrentTime;
	mFramesPerSecond = 1.f / mDeltaTime;

	//std::cout << "deltatime: " << mDeltaTime << "\n";

	//auto duration = std::chrono::steady_clock::now().time_since_epoch();
	//auto count = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();

	//mTick = 1000000 / FPS;
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

bool Core::KeyUp(SDL_Scancode k)
{
	if (mpKeyStates != nullptr)
	{
		if (mpKeyStates[k] == 0)
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

void Core::HandleEvents()
{
	ImGuiIO& io = ImGui::GetIO();

	if (mLMBState == true)
		mLMBState = false;

	while (SDL_PollEvent(&mEvent))
	{
		//Does most of the input/event processing.
		ImGui_ImplSDL2_ProcessEvent(&mEvent);

		if (mEvent.type == SDL_WINDOWEVENT)
		{
			switch (mEvent.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				GameInstance::Instance()->AddLog("Window resized.\n");
				break;

			case SDL_WINDOWEVENT_FOCUS_LOST:
				if (!mpAM->IsPaused())
				{
					mMusicEnabled = false;
					mpAM->PauseMusic();
				}
				else
					mMusicEnabled = true;
				break;

			case SDL_WINDOWEVENT_FOCUS_GAINED:
				if (mpAM->IsPaused() && mMusicEnabled == false)
					mpAM->ResumeMusic();
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
				{
					mLMBState = true;
					mLMBDown = true;
				}

			if (mEvent.button.button == SDL_BUTTON_MIDDLE)
				if (mEvent.button.state == SDL_PRESSED)
				{
					//std::cout << "hi\n";
				}
			break;

		case SDL_MOUSEBUTTONUP:
			if (mEvent.button.button == SDL_BUTTON_LEFT)
			{
				mLMBDown = false;
			}
			break;

		case SDL_MOUSEMOTION:
			mMousePosX = mEvent.motion.x;
			mMousePosY = mEvent.motion.y;
			mMouseRelX = mEvent.motion.xrel;
			mMouseRelY = mEvent.motion.yrel;
			break;
		}
	}

	mpFSM->HandleStateEvents(&mEvent);
}

void Core::Update(float deltaTime)
{
	mpFSM->Update(deltaTime);
}

void Core::Render()
{
	mpFSM->Render();
}

void Core::LateUpdate(float deltaTime)
{
	mpFSM->LateUpdate(deltaTime);
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

	//Clean-up SDL.
	SDL_DestroyRenderer(mpSDLRenderer);
	SDL_GL_DeleteContext(mGLContext);
	SDL_DestroyWindow(mpSDLWindow);

	delete mpFSM;
	delete mpAM;

	mpFSM = nullptr;
	mpAM = nullptr;

	delete mspInstance;
	mspInstance = nullptr;

	GameInstance::Instance()->ResetInstance();
	ECSHandler::Instance()->ResetInstance();

	mpSDLWindow = nullptr;
	mpSDLRenderer = nullptr;

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
