#include "AudioManager.h"

#include "GameInstance.h"

AudioManager::~AudioManager()
{
	GameInstance::Instance()->AddLog("Destroying the audio manager...\n");
	ClearMusic();
	ClearSounds();
}

void AudioManager::SetMusicVolume(int volume)
{
	volume = (volume = 0 ? 0 : (volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume));
	Mix_VolumeMusic(volume);
}

void AudioManager::SetSoundsVolume(int idx, int volume)
{
	volume = (volume = 0 ? 0 : (volume > MIX_MAX_VOLUME ? MIX_MAX_VOLUME : volume));
	Mix_VolumeChunk(mSounds[idx], volume);
}

void AudioManager::PlayMusic(int idx, int loops)
{
	Mix_PlayMusic(mMusic[idx], loops);
}

void AudioManager::PlaySound(int idx, int channel, int loops)
{
	//std::cout << "sound played\n";
	//Mix_HaltChannel(channel);
	Mix_PlayChannel(channel, mSounds[idx], loops);
}

void AudioManager::LoadMusic(const char* file)
{
	Mix_Music* music = Mix_LoadMUS(file);
	if (music != nullptr)
		mMusic.push_back(music);
	else
		GameInstance::Instance()->AddLog("[FAIL] Music Loading.\n");
}

void AudioManager::LoadSound(const char* file)
{
	Mix_Chunk* sound = Mix_LoadWAV(file);
	if (sound != nullptr)
		mSounds.push_back(sound);
	else
		GameInstance::Instance()->AddLog("[FAIL] Sound Loading.\n");
}

void AudioManager::ToggleMusic()
{
	if (Mix_PausedMusic() == 1)
		Mix_ResumeMusic();
	else
		Mix_PauseMusic();
}

void AudioManager::ResumeMusic()
{
	Mix_ResumeMusic();
}

void AudioManager::PauseMusic()
{
	Mix_PauseMusic();
}

bool AudioManager::IsPaused()
{
	if (Mix_PausedMusic() == 1)
		return true;
	else
		return false;
}

void AudioManager::ClearMusic()
{
	for (int i = 0; i < (int)mMusic.size(); i++)
	{
		Mix_FreeMusic(mMusic[i]);
		mMusic[i] = nullptr;
	}
	mMusic.clear();
	mMusic.shrink_to_fit();
}

void AudioManager::ClearSounds()
{
	for (int i = 0; i < (int)mSounds.size(); i++)
	{
		Mix_FreeChunk(mSounds[i]);
		mSounds[i] = nullptr;
	}
	mSounds.clear();
	mSounds.shrink_to_fit();
}

void AudioManager::ClearNewestSound()
{
	if (!mSounds.empty())
	{
		Mix_FreeChunk(mSounds.back());
		mSounds.back() = nullptr;
		mSounds.shrink_to_fit();
	}
	else
		GameInstance::Instance()->AddLog("Unable to clear latest sound. mSounds vector is empty.\n");
}
