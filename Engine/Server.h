#pragma once
//#include <unordered_map>
//#include <vector>

#include "../NetworkLibrary/MyProtocol.h"

class ServerSocket;
class Session;
class ClientNetworkManager;
class WinSock;

typedef int SessionId;

/// <summary>
/// 서버
/// 클라이언트의 정보를 들고 있으며 클라이언트와 연결을 진행한다.
/// 게임 상태, 로비 상태 가지고 있기.
/// </summary>

class ServerNetworkManager
{
public:
	ServerNetworkManager() = default;
	~ServerNetworkManager() = default;

	void Start(const char* add);
	void Stop();
	void CleanUp();

	void Update();
	void NetUpdate();

	void OnAccept();
	void OnReceive(std::shared_ptr<ClientNetworkManager> pClient);
	void OnSend(std::shared_ptr<WinSock> pSocket);
	void OnClose(std::shared_ptr<ClientNetworkManager> pClient);
	void onNetError(int errorCode, const char* errorMsg, std::shared_ptr<WinSock> pSocket = nullptr);

	char* SerializeBuffer(int size, EPacketId id, char* msg);
	void DeSerializeBuffer(PacketHeader& packet, char* buf);

	void CheckId(std::shared_ptr<Session> session, char* pData, int len);
	void BroadcastPacket(char* pData, int len);

	// 게임 로직 관련 함수들
public:
	void ClientSetReady(std::shared_ptr<Session> session, char* pMsg);
	void BroadcastAction(std::shared_ptr<Session> session, char* pMsg);
	void SetTurn(std::shared_ptr<Session> session, char* pMsg);
	void MovementEnd(std::shared_ptr<Session> session, char* pMsg);
	//void EndTurn(std::shared_ptr<Session> session, char* pMsg);
	void ChangeTurn();
	void StartTurn(std::shared_ptr<Session> session, char* pMsg);
	void BroadcastCheckAction();
	void CheckRestartState(std::shared_ptr<Session> session, char* pMsg);
	void BroadcastMsg(std::shared_ptr<Session> session, char* pMsg);
	void ClientSetEnter(std::shared_ptr<Session> session, char* pMsg);
	void IsAllGoalIn(std::shared_ptr<Session> session, char* pMsg);
	void IsAllGoalEnd(std::shared_ptr<Session> session, char* pMsg);
	void IsAllEnter();
	void IsAllReady();

	void ServerLoop();

private:
	std::shared_ptr<ServerSocket> m_pListenSocket;

	std::vector<std::shared_ptr<ClientNetworkManager>> m_peerClients;
	std::unordered_map<SessionId, std::shared_ptr<Session>> m_sessions;

	int m_ClientCount = 0;

	HANDLE	m_hQuitEvent;

	bool m_bAllReady = false;
	bool m_bAllEnd = false;
	bool m_bRun = false;
	char m_currentTurn;
};

