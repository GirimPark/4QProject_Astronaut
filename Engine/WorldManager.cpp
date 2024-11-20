#include "pch.h"
#include "WorldManager.h"

#include "CommonApp.h"

#include <Winsock2.h>

void WorldManager::ChangeWorld(eWorldType worldType)
{
	CommonApp::m_pInstance->GetRenderer()->ClearWorldResource();

	std::pair<char*, int> value1;
	while (m_recvQueue.try_pop(value1))
	{
		delete[] value1.first;
	}
	std::pair<char*, int> value2;
	while (m_recvQueue.try_pop(value2))
	{
		delete[] value2.first;
	}

	m_prevWorld = m_currentWorld;
	assert(m_worlds[static_cast<int>(worldType)]);
	m_currentWorld = m_worlds[static_cast<int>(worldType)];

	m_prevWorld->OnExit();
	m_currentWorld->OnEnter();
}

void WorldManager::GoToPrevWorld()
{
	CommonApp::m_pInstance->GetRenderer()->ClearWorldResource();

	std::shared_ptr<World> temp = m_prevWorld;
	m_prevWorld = m_currentWorld;
	m_currentWorld = temp;

	m_prevWorld->OnExit();
	m_currentWorld->OnEnter();
}

void WorldManager::ProcessPacket(char* pData, PacketHeader& packet) 
{
	EPacketId id = packet.id;
	auto it = m_handlers.find(id);
	if (it != m_handlers.end()) 
	{
		(it->second)(pData);
	}
}

void WorldManager::Initialize()
{
	m_currentWorld->OnEnter();
}

void WorldManager::Update(float deltaTime)
{
	std::pair<char*, int> value;
	while (m_recvQueue.try_pop(value))
	{
		PacketHeader packet;
		DeSerializeBuffer(packet, value.first);

		ProcessPacket(value.first, packet);
	}

	m_currentWorld->Update(deltaTime);
}

void WorldManager::Finalize()
{
	for(auto& world:m_worlds)
	{
		world.reset();
	}

	while (!m_recvQueue.empty())
	{
		std::pair<char*, int> value;

		if (m_recvQueue.try_pop(value))
		{
			delete[] value.first;
		}
	}

	while (!m_sendQueue.empty())
	{
		std::pair<char*, int> value;
		if (m_sendQueue.try_pop(value))
		{
			delete[] value.first;
		}
	}
	Singleton<WorldManager>::Finalize();
}

// Serialize 진행하기
char* WorldManager::SerializeBuffer(short size, EPacketId id, char* msg)
{
	if (id <= PACKETID_START || id >= PACKETID_END)
		return nullptr;

	char* buf = new char[size + 1];

	PacketHeader ph;
	ph.size = size;
	ph.id = id;

	memcpy(buf, &ph, sizeof(PacketHeader));

	if (msg == nullptr)
		return buf;

	memcpy(buf + sizeof(PacketHeader), msg, size - sizeof(PacketHeader));

	return buf;
}

void WorldManager::DeSerializeBuffer(PacketHeader& packet, char* buf)
{
	short sizeInfo = 0;
	memcpy(&sizeInfo, buf, sizeof(short));

	EPacketId idInfo;
	memcpy(&idInfo, buf + sizeof(short), sizeof(EPacketId));

	packet.size = sizeInfo;
	packet.id = idInfo;
}
