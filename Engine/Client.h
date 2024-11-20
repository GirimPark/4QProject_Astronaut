#pragma once
#define MAX_LEN 100
#include "../NetworkLibrary/MyProtocol.h"

#include <memory>

#define THREAD_DEBUG

class ClientSocket;
class WinSock;

typedef int SessionId;

class ClientNetworkManager
{
public:
	ClientNetworkManager() = default;
	~ClientNetworkManager() = default;
	
	std::shared_ptr<ClientSocket> GetSocket() { return m_clientSocket; }
	void SetSocket(std::shared_ptr<ClientSocket> socket) { m_clientSocket = socket; }

	void Start(const char* add);
	void Update();
	void NetUpdate();
	void Stop();

	void CleanUp();
	// recv해서 받은 버퍼가 유효한 패킷인지 확인하기.
	void Read(char* buf);
	void Write(char* buf, int len);

	void OnNetError(int errorCode, const char* errorMsg, std::shared_ptr<WinSock> pSocket);

	void OnReceive();
	void OnSend();
	void OnClose();

	void SetSessionId(SessionId sessionId) { m_sessionId = sessionId; }
	SessionId GetSessionId() { return m_sessionId; }

	void ClientLoop();

private:
	std::shared_ptr<ClientSocket> m_clientSocket;
	SessionId m_sessionId = 0;

	char m_sendBuffer[SND_BUF_SIZE];
	char m_recvBuffer[RCV_BUF_SIZE];

	int m_recvBytes = 0;
	int m_sendBytes = 0;

	HANDLE	m_hQuitEvent;

	bool m_bRun = false;
	int m_connectResult;
	const char* m_address;
};

