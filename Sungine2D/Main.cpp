#include "Core.h"

int main(int argc, char* args[])
{
	Core::Instance()->InitAll("Sungine2D", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Core::Instance()->GetWindowWidth(),
		Core::Instance()->GetWindowHeight(), SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN/* | SDL_WINDOW_RESIZABLE*/);

	while (Core::Instance()->IsRunning())
	{
		SDL_PumpEvents();
		Core::Instance()->HandleEvents();
		
		if (Core::Instance()->Tick())
		{
			Core::Instance()->Update(Core::Instance()->GetDeltaTime());
			Core::Instance()->Render();
			Core::Instance()->LateUpdate(Core::Instance()->GetDeltaTime());
		}
	}

	Core::Instance()->Clean();

	return 0;
}