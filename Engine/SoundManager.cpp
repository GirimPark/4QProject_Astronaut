#include "pch.h"
#include "SoundManager.h"
#include "fmod_errors.h"

inline void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		LOG_ENGINE(FMOD_ErrorString(result));
		assert(false);
	}
}

SoundManager::~SoundManager()
{
}

FMOD::Sound* SoundManager::LoadSound(std::string filePath, FMOD_MODE mode)
{
	FMOD::Sound* sound;

	result = m_system->createSound(filePath.c_str(), mode, nullptr, &sound);
	ERRCHECK(result);

	m_soundMap[filePath] = sound;

	return sound;
}

void SoundManager::StopAllSound()
{
	for (auto& chennel : m_playingchannels)
	{
		chennel->setPaused(true);
	}
}

void SoundManager::PlaySound(FMOD::Sound* sound)
{
	FMOD::Channel* channel;
	result = m_system->playSound(sound, nullptr, false, &channel);
	ERRCHECK(result);

	m_playingchannels.push_back(channel);
}

void SoundManager::PlaySound(std::string filePath)
{
	auto it = m_soundMap.find(filePath);

	if (it != m_soundMap.end())
	{
		PlaySound(it->second);
	}
}

void SoundManager::TogglePlay(FMOD::Sound* sound)
{
	auto channl = FindChannel(sound);
	bool paused;
	channl->getPaused(&paused);
	channl->setPaused(!paused);
}

void SoundManager::TogglePlay(std::string filePath)
{
	auto it = m_soundMap.find(filePath);

	if (it != m_soundMap.end())
	{
		TogglePlay(it->second);
	}
}

void SoundManager::SetVolume(FMOD::Sound* sound, float volume)
{
	auto channl = FindChannel(sound);
	channl->setVolume(volume);
}

void SoundManager::SetVolume(std::string filePath, float volume)
{
	auto it = m_soundMap.find(filePath);

	if (it != m_soundMap.end())
	{
		SetVolume(it->second, volume);
	}
}

void SoundManager::Initailize()
{
	result = FMOD::System_Create(&m_system);
	ERRCHECK(result);
	result = m_system->init(32, FMOD_INIT_NORMAL, nullptr);
	ERRCHECK(result);
}

void SoundManager::Update(float deltaTime)
{
	result = m_system->update();
	ERRCHECK(result);

	m_playingchannels.remove_if([](const auto& channel){
		bool playing;
		channel->isPlaying(&playing);
		return !playing;
		});

}

void SoundManager::Finalize()
{
	for (auto& sound : m_soundMap)
	{
		sound.second->release();
	}
	m_soundMap.clear();
	m_playingchannels.clear();
	m_system->close();
	m_system->release();
	delete m_pInstance;
}

FMOD::Channel* SoundManager::FindChannel(FMOD::Sound* sound)
{
	FMOD::Sound* _sound;
	FMOD::Sound* __sound = sound;

	for (auto& _channel : m_playingchannels)
	{
		_channel->getCurrentSound(&_sound);

		if (_sound == __sound)
		{
			return _channel;
		}
	}

	return nullptr;
}
