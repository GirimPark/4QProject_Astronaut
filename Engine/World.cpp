#include "pch.h"
#include "World.h"

constexpr int resreveSize = 30;

World::World()
{
	for(auto& gameObjects : m_gameObjects)
	{
		gameObjects.reserve(resreveSize);
	}
	m_objectMap.reserve(resreveSize * static_cast<int>(eObjectType::END));

	m_UIManager = std::make_unique<UIManager>();
}

World::~World()
{
	World::Finalize();
}

std::shared_ptr<GameObject> World::GetGameObject(std::string_view name)
{
	std::pair<eObjectType, int> pair = m_objectMap.find(std::string(name))->second;

	return m_gameObjects[static_cast<int>(pair.first)][pair.second];
}

void World::DeleteGameObject(std::string_view name)
{
	auto mapIter = m_objectMap.find(std::string(name));
	if (mapIter == m_objectMap.end())
	{
		return;
		LOG_ENGINE("이 오브젝트 없어용");
	}

	std::pair<eObjectType, int> pair = mapIter->second;
	m_objectMap.erase(std::string(name));

	auto iter = m_gameObjects[static_cast<int>(pair.first)].begin() + pair.second;
	iter = m_gameObjects[static_cast<int>(pair.first)].erase(iter);

	while(iter != m_gameObjects[static_cast<int>(pair.first)].end())
	{
		m_objectMap.find((*iter)->GetName())->second.second--;
		++iter;
	}
}

void World::DeleteGameObject(const std::shared_ptr<GameObject> gameObj)
{
	std::string name = gameObj->GetName();
	auto mapIter = m_objectMap.find(std::string(name));
	if (mapIter == m_objectMap.end())
	{
		return;
		LOG_ENGINE("이 오브젝트 없어용");
	}

	std::pair<eObjectType, int> pair = mapIter->second;
	m_objectMap.erase(std::string(name));

	auto iter = m_gameObjects[static_cast<int>(pair.first)].begin() + pair.second;
	iter = m_gameObjects[static_cast<int>(pair.first)].erase(iter);

	while (iter != m_gameObjects[static_cast<int>(pair.first)].end())
	{
		m_objectMap.find((*iter)->GetName())->second.second--;
		++iter;
	}
}

void World::Update(float deltaTime)
{
	m_UIManager->Update(deltaTime);

	for (auto& gameObjectGroup : m_gameObjects)
	{
		for (const auto& gameObject : gameObjectGroup)
		{
			gameObject->Update(deltaTime);
		}
	}
}

void World::OnEnter()
{
	this->Initialize();
}

void World::OnExit()
{
	this->Finalize();
}

std::string World::IsRegisteredName(std::string_view name)
{
	std::string returnName = std::string(name);
	while(m_objectMap.find(returnName) != m_objectMap.end())
		returnName += "+";

	return returnName;
}

void World::Initialize()
{
	m_UIManager->Initailize();

	for (auto& gameObjectGroup : m_gameObjects)
	{
		for (const auto& gameObject : gameObjectGroup)
		{
			gameObject->Initialize();
		}
	}
}

void World::Finalize()
{
	for (auto& gameObjectGroup : m_gameObjects)
	{
		for (auto& gameObject : gameObjectGroup)
			gameObject.reset();

		gameObjectGroup.clear();
	}

	m_objectMap.clear();

	//m_UIManager->Finalize();
}
