#pragma once
#include "fmod.hpp"
#include "Singleton.h"

class SoundManager
	: public Singleton<SoundManager>
{
public:
	~SoundManager();

	FMOD::Sound* LoadSound(std::string filePath, FMOD_MODE mode = FMOD_DEFAULT);
	void StopAllSound();

	void PlaySound(FMOD::Sound* sound);
	void PlaySound(std::string filePath);

	void TogglePlay(FMOD::Sound* sound);
	void TogglePlay(std::string filePath);

	void SetVolume(FMOD::Sound* sound, float volume);
	void SetVolume(std::string filePath, float volume);

	void Initailize();
	void Update(float deltaTime);
	void Finalize() override;

private:
	FMOD::Channel* FindChannel(FMOD::Sound* sound);

	FMOD::System* m_system;
	std::unordered_map<std::string, FMOD::Sound*> m_soundMap;
	std::list<FMOD::Channel*> m_playingchannels;

	FMOD_RESULT result;
};

