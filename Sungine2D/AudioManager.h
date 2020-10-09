#pragma once

#include <vector>

//SDL
#include "SDL_mixer.h"

namespace Sungine
{
	class AudioManager
	{
	private:
		std::vector<Mix_Music*> mMusic;
		std::vector<Mix_Chunk*> mSounds;

	public:
		~AudioManager();

		void SetMusicVolume(int volume);
		void SetSoundsVolume(int idx, int volume);
		void PlayMusic(int idx = 0, int loops = -1);
		void PlaySound(int idx, int channel = -1, int loops = 0);
		void LoadMusic(const char* file);
		void LoadSound(const char* file);
		void ToggleMusic();
		void ResumeMusic();
		void PauseMusic();
		bool IsPaused();
		void ClearMusic();
		void ClearSounds();
		void ClearNewestSound();
	};
}

