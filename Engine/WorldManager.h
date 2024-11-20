#pragma once

#include "World.h"
#include "Singleton.h"

#include "../NetworkLibrary/MyProtocol.h"

#include <concurrent_queue.h>

///<summary>
///World를 만들어 등록하면 worldManager를 통해 init, update된다.
///</summary>
/// todo 채원 : 월드 체인지 할 때마다 큐 비워주기.

using PacketHandler = std::function<void(char*)>;

class WorldManager
	: public Singleton<WorldManager>
{
public:
	WorldManager() = default;
	~WorldManager() = default;

public:
	std::shared_ptr<World> GetCurWorld() { return m_currentWorld; }

	void SetDefaultWorld(std::shared_ptr<World> world) { m_currentWorld = world; }

	void ChangeWorld(eWorldType worldType);
	void GoToPrevWorld();

	bool PopRecvQueue(std::pair<char*, int>& value) { return m_recvQueue.try_pop(value); }
	bool PopSendQueue(std::pair<char*, int>& value) { return m_sendQueue.try_pop(value); }

	void PushRecvQueue(char* pBuf, int len) { m_recvQueue.push({ pBuf, len }); }
	void PushSendQueue(char* pBuf, int len) { m_sendQueue.push({ pBuf, len }); }
	
	bool IsRecvQueueEmpty() { return m_recvQueue.empty(); }
	bool IsSendQueueEmpty() { return m_sendQueue.empty(); }

	char* SerializeBuffer(short size, EPacketId id, char* msg);
	void DeSerializeBuffer(PacketHeader& packet, char* buf);

	void RegisterHandler(EPacketId packetID, PacketHandler handler) { m_handlers[packetID] = handler; }
	void ProcessPacket(char* pData, PacketHeader& packet);

	void Initialize();
	void Update(float deltaTime);
	void Finalize() override;

public:
	void SetHostServer() { m_bHost = true; }
	bool IsHostServer() { return m_bHost; }

private:
	std::shared_ptr<World> m_worlds[static_cast<int>(eWorldType::END)];
	std::shared_ptr<World> m_currentWorld;
	std::shared_ptr<World> m_prevWorld;

	// 버퍼랑 버퍼의 length값 갖고 있는 큐
	Concurrency::concurrent_queue<std::pair<char*, int>> m_sendQueue;
	Concurrency::concurrent_queue<std::pair<char*, int>> m_recvQueue;

	std::unordered_map<EPacketId, PacketHandler> m_handlers;

	int m_sendBytes = 0;
	int m_recvBytes = 0;

	bool m_bHost = false;

public:
	template <typename T>
	std::weak_ptr<T> RegisterWorld(std::string_view worldName, eWorldType worldType)
	{
		std::shared_ptr<T> world = std::make_shared<T>();

		World* inputWorld = static_cast<World*>(world.get());
		assert(inputWorld);

		world->SetName(worldName);
		world->SetWorldType(worldType);

		m_worlds[static_cast<int>(worldType)] = world;

		return world;
	}
};

