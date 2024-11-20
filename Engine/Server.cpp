#include "pch.h"
#include "Server.h"

#include "Client.h"
#include "Session.h"

#include "../NetworkLibrary/ClientSocket.h"
#include "../NetworkLibrary/ServerSocket.h"
#include "../NetworkLibrary/WinSock.h"
#include "../Engine/WorldManager.h"

void ServerNetworkManager::Start(const char* add)
{
	WSAData wsaData;

	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return;

	m_pListenSocket = std::make_shared<ServerSocket>();

	if (!m_pListenSocket->Create(SOCK_STREAM, FD_ACCEPT)
		|| !m_pListenSocket->Bind(7777, add)
		|| !m_pListenSocket->Listen())
		return;

	// 종료이벤트 핸들
	m_hQuitEvent = CreateEvent(NULL, false, false, NULL);
	m_bRun = true;
}

void ServerNetworkManager::Stop()
{
	if (m_hQuitEvent != INVALID_HANDLE_VALUE)
		SetEvent(m_hQuitEvent);
}

void ServerNetworkManager::CleanUp()
{
	if (m_hQuitEvent != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hQuitEvent);
	}
	::WSACleanup();
}

void ServerNetworkManager::Update()
{
	std::vector<WSAEVENT> wsaEvents;

	// 리슨 소켓의 이벤트 개체 관리
	wsaEvents.emplace_back(m_pListenSocket->GetEvent());

	for (const auto& pClient : m_peerClients)
	{
		if (pClient)
		{
			// peerClient 소켓의 이벤트 개체 관리
			std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
			wsaEvents.emplace_back(soc->GetEvent());
		}
	}

	if (WAIT_TIMEOUT != WaitForSingleObject(m_hQuitEvent, 0))
	{
		// Quit 이벤트가 발생했다면 실행 중단
		m_bRun = false;
		return;
	}

	int index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);

	if (index == WSA_WAIT_FAILED)
	{
		printf("WSAWaitForMultipleEvents Error %d", WSAGetLastError());
		return;
	}

	if (index == WSA_WAIT_TIMEOUT) return;

	index -= WSA_WAIT_EVENT_0;

	// 이벤트가 발생한 소켓 확인
	std::shared_ptr<WinSock> pSocket = nullptr;

	if (wsaEvents[index] == m_pListenSocket->GetEvent())
		pSocket = m_pListenSocket;
	else
		pSocket = m_peerClients[index - 1]->GetSocket();

	WSANETWORKEVENTS networkEvents;

	if (::WSAEnumNetworkEvents(pSocket->GetHandle(), wsaEvents[index], &networkEvents) == SOCKET_ERROR)
	{
		printf("WSAEnumNetworkEvents Error %d", WSAGetLastError());

		return;
	}

	if (networkEvents.lNetworkEvents & FD_ACCEPT)
	{
		if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_ACCEPT_BIT], "Accept");
			return;
		}

		OnAccept();
	}

	if (networkEvents.lNetworkEvents & FD_READ)
	{
		if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_READ_BIT], "Recv", pSocket);
			return;
		}

		OnReceive(m_peerClients[index - 1]);
	}

	if (networkEvents.lNetworkEvents & FD_WRITE)
	{
		if (networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Write", pSocket);
			return;
		}

		OnSend(pSocket);
	}

	if (networkEvents.lNetworkEvents & FD_CLOSE)
	{
		if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_CLOSE_BIT], "Close", pSocket);
		}

		OnClose(m_peerClients[index - 1]);
	}
}

void ServerNetworkManager::NetUpdate()
{
	for (auto& session : m_sessions)
	{
		std::pair<char*, int> recvValue;
		while (session.second->PopRecvQueue(recvValue))
		{
			CheckId(session.second, recvValue.first, recvValue.second);
			delete[] recvValue.first;
		}

		std::pair<char*, int> sendValue;
		while (session.second->PopSendQueue(sendValue))
		{
			int nSent = session.second->Send(sendValue.first, sendValue.second);
			delete[] sendValue.first;
		}
	}
}

void ServerNetworkManager::OnAccept()
{
	std::shared_ptr<ClientNetworkManager> pClient = std::make_shared<ClientNetworkManager>();
	pClient->SetSocket(std::make_shared<ClientSocket>());
	std::shared_ptr<Session> pSession = std::make_shared<Session>();

	std::shared_ptr<ClientSocket> clientSocket = pClient->GetSocket();

	if (m_pListenSocket->OnAccept(clientSocket))
	{
		pSession->SetClient(pClient);

		m_peerClients.emplace_back(pClient);
		m_sessions[pSession->GetSessionId()] = pSession;

		++m_ClientCount;

		printf("Server : Accept Client\n");
	}
	else
	{
		pClient.reset();
		pSession.reset();

		printf("Accept Error %d\n", WSAGetLastError());
	}
}

void ServerNetworkManager::OnReceive(std::shared_ptr<ClientNetworkManager> pClient)
{
	if (pClient->GetSocket() == nullptr) return;

	printf("onReceive  %s : %d\n", pClient->GetSocket()->GetIP().c_str(), pClient->GetSocket()->GetPort());


	std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
	if (pSession == nullptr) return;

	pSession->ReadUpdate();

	if (pSession->IsRecvQueueEmpty())
	{
		return;
	}
}

void ServerNetworkManager::OnSend(std::shared_ptr<WinSock> pSocket)
{
	printf("onSend  %s : %d\n", pSocket->GetIP().c_str(), pSocket->GetPort());
}

void ServerNetworkManager::OnClose(std::shared_ptr<ClientNetworkManager> pClient)
{
	if (pClient->GetSocket() == nullptr) return;

	// sendQueue, recvQueue 정리
	m_sessions[pClient->GetSessionId()].get()->Finalize();

	m_sessions.erase(pClient->GetSessionId());

	for (auto iter = m_peerClients.begin(); iter != m_peerClients.end(); ++iter)
	{
		if (iter->get() == pClient.get())
		{
			m_peerClients.erase(iter);
			break;
		}
	}
	m_ClientCount--;
	printf("연결된 클라이언트 수 : %d \n", m_ClientCount);
}

void ServerNetworkManager::onNetError(int errorCode, const char* errorMsg, std::shared_ptr<WinSock> pSocket)
{
	if (errorMsg)
	{
		printf("\n onNetError %s \t", errorMsg);
	}

	if (pSocket)
	{
		printf("\n onNetError  %s : %d", pSocket->GetIP().c_str(), pSocket->GetPort());
	}

	printf("NetErrorCode  %d  \n", errorCode);
}

char* ServerNetworkManager::SerializeBuffer(int size, EPacketId id, char* msg)
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

void ServerNetworkManager::DeSerializeBuffer(PacketHeader& packet, char* buf)
{
	short sizeInfo = 0;
	memcpy(&sizeInfo, buf, sizeof(short));

	EPacketId idInfo;
	memcpy(&idInfo, buf + sizeof(short), sizeof(EPacketId));

	packet.size = sizeInfo;
	packet.id = idInfo;
}

// 게임 로직들
void ServerNetworkManager::CheckId(std::shared_ptr<Session> session, char* pData, int len)
{
	PacketHeader pHeader;
	DeSerializeBuffer(pHeader, pData);

	switch (pHeader.id)
	{
	case C2S_IS_ALL_GOAL:
		IsAllGoalIn(session, pData);
		break;

	case C2S_IS_ALL_END:
		IsAllGoalEnd(session, pData);
		break;

	case C2S_IS_ALL_ENTER:
		ClientSetEnter(session, pData);
		IsAllEnter();
		break;
	
	case C2S_READY:
		ClientSetReady(session, pData);
		//IsAllReady();
		break;

	case C2S_IAM_HOST_MSG:
		session->SetHostSession();
		break;

	case C2S_BROADCAST_ACTION:
		BroadcastAction(session, pData);
		break;

	case C2S_SET_TURN:
		SetTurn(session, pData);
		break;

	case C2S_MOVEMENT_END:
		MovementEnd(session, pData);
		break;

	/*case C2S_CHANGE_TURN:
		ChangeTurn();
		break;*/

	case C2S_START_TURN:
		StartTurn(session, pData);
		break;

	case C2S_CHECK_ACTION:
		BroadcastCheckAction();
		break;

	case C2S_RESTART_GAME:
		CheckRestartState(session, pData);
		break;

	case C2S_BROADCAST_MSG:
		BroadcastMsg(session, pData);
		break;

	default:
		break;
	}
}

void ServerNetworkManager::BroadcastPacket(char* pData, int len)
{
	for (auto& session : m_sessions)
	{
		char* sendData = new char[len + 1];
		if (pData != nullptr)
			memcpy(sendData, pData, len);
		session.second->PushSendQueue(sendData, len);
	}
	delete pData;
}

void ServerNetworkManager::BroadcastMsg(std::shared_ptr<Session> session, char* pMsg)
{
	char* broadcastMsg = new char[sizeof(PacketS2C_BroadcastMsg)+ 1];
	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_BroadcastMsg), S2C_BROADCAST_MSG, broadcastMsg), sizeof(PacketS2C_BroadcastMsg));
}

void ServerNetworkManager::ClientSetEnter(std::shared_ptr<Session> session, char* pMsg)
{
	session->SetEnterState(true);
}

void ServerNetworkManager::IsAllGoalIn(std::shared_ptr<Session> session, char* pMsg)
{
	if (m_peerClients.size() <= 1)
		return;

	session->SetGoalIn(true);

	for (auto& pClient : m_peerClients)
	{
		if (pClient == nullptr)
		{
			continue;
		}
		else
		{
			std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
			std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];

			if (pSession == nullptr) return;
			if (!pSession->GetGoalIn())	return;
		}
	}

	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_IsAllGoal), S2C_IS_ALL_GOAL, nullptr), sizeof(PacketS2C_IsAllGoal));

	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];

		if (pSession == nullptr) return;

		pSession->SetGoalIn(false);
	}

}

void ServerNetworkManager::IsAllGoalEnd(std::shared_ptr<Session> session, char* pMsg)
{
	if (m_peerClients.size() <= 1)
		return;

	session->SetGoalEnd(true);

	for (auto& pClient : m_peerClients)
	{
		if (pClient == nullptr)
		{
			continue;
		}
		else
		{
			std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
			std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];

			if (pSession == nullptr) return;
			if (!pSession->GetGoalEnd())	return;
		}
	}

	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_IsAllEnd), S2C_IS_ALL_END, nullptr), sizeof(PacketS2C_IsAllEnd));

	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];

		if (pSession == nullptr) return;

		pSession->SetGoalEnd(false);
	}

}

void ServerNetworkManager::ClientSetReady(std::shared_ptr<Session> session, char* pMsg)
{
	if (m_peerClients.size() <= 1)
		return;

	PacketC2S_READY* pSetReady = reinterpret_cast<PacketC2S_READY*>(pMsg);
	assert(pSetReady != nullptr);

	session->SetReady(true, true);

	// 세션 다 들어왔는지 확인
	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			// 서버에 안 들어온게 있다면
			if (!pSession->GetReady().bserverEnter) return;
		}
	}

	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_IsAllReady), S2C_IS_ALL_READY, nullptr), sizeof(PacketS2C_IsAllReady));

	// 다 들어왔는지 체크 됐으면 false로 바꿔주기
	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			pSession->SetReady(false, false);
		}
	}	
}

void ServerNetworkManager::BroadcastAction(std::shared_ptr<Session> session, char* pMsg)
{
	PacketC2S_BroadcastAction* pBroadcastAction = reinterpret_cast<PacketC2S_BroadcastAction*>(pMsg);

	PacketS2C_BroadcastAction ba;
	ba.who = pBroadcastAction->who;
	for (int i = 0; i < 4; i++)
	{
		ba.action[i] = pBroadcastAction->action[i];
	}
	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_BroadcastAction), S2C_BROADCAST_ACTION, &ba.who), sizeof(PacketS2C_BroadcastAction));
}

void ServerNetworkManager::SetTurn(std::shared_ptr<Session> session, char* pMsg)
{
	PacketC2S_SetTurn* pSetTurn = reinterpret_cast<PacketC2S_SetTurn*>(pMsg);
	assert(pSetTurn != nullptr);

	if (pSetTurn->who == '0') // 호스트 턴
	{
		session->SetSessionTurn('0', true);
	}
	else if (pSetTurn->who == '1') // 세션이 게스트면 게스트한테 턴 세팅해주기
	{
		session->SetSessionTurn('1', true);
	}
	
	if (m_peerClients.size() <= 1)
		return;

	// 세션 다 들어왔는지 확인
	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			// 서버에 안 들어온게 있다면
			if (!pSession->GetSessionTurn().bserverEnter) return;
		}
	}

	// 세션이 다 들어왔으면 브로드캐스트
	PacketS2C_SetTurn st;
	
	st.setTurn = session->GetSessionTurn().bHost;

	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_SetTurn), S2C_SET_TURN, &st.setTurn),
			sizeof(PacketS2C_SetTurn));

	// 다 들어왔는지 체크 됐으면 false로 바꿔주기
	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			pSession->SetSessionTurn('0', false);
		}
	}
}

// 플레이어의 행동이 끝났는지 확인
void ServerNetworkManager::MovementEnd(std::shared_ptr<Session> session, char* pMsg)
{
	// 움직임 끝난게 호스트 플레이어인지 게스트 플레이어인지
	PacketC2S_MovementEnd* pMovementEnd = reinterpret_cast<PacketC2S_MovementEnd*>(pMsg);
	if(pMovementEnd->who == '0')
		session->SetMovement(true);
	else if(pMovementEnd->who == '1')
		session->SetMovement(false);

	// 한 세션의 플레이어들이 다 움직였다
	if (session->GetAllMovementEnd())
	{
		session->SetMovementFalse();
		session->SetSessionMovementEnd(true);
	}

	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			// 아직 플레이어의 움직임이 끝나지 않은 곳이 있다면 return
			if (!pSession->GetSessionMovementEnd()) return;
		}
	}

	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_MovementEnd), S2C_MOVEMENT_END, nullptr),
		sizeof(PacketS2C_MovementEnd));

	// MovementEnd 초기화
	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			// 아직 플레이어의 움직임이 끝나지 않은 곳이 있다면 return
			pSession->SetSessionMovementEnd(false);
		}
	}
}

void ServerNetworkManager::ChangeTurn()
{
	/*static short sessionCnt = 0;
	sessionCnt++;
	if (sessionCnt == 2)
	{
		BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_ChangeTurn), S2C_CHANGE_TURN, nullptr), sizeof(PacketS2C_ChangeTurn));
		sessionCnt = 0;
	}*/
}

void ServerNetworkManager::StartTurn(std::shared_ptr<Session> session, char* pMsg)
{
	/*session->PushSendQueue(
		SerializeBuffer(sizeof(PacketS2C_StartGame), S2C_START_TURN, nullptr),
		sizeof(PacketS2C_StartGame));*/
	session->SetStartTurn(true);

	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			// 서버에 안 들어온게 있다면
			if (!pSession->GetStartTurn().bserverEnter) return;
		}
	}

	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_StartGame), S2C_START_TURN, nullptr),
		sizeof(PacketS2C_StartGame));

	// 다 들어왔는지 체크 됐으면 false로 바꿔주기
	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			pSession->SetStartTurn(false);
		}
	}
}

void ServerNetworkManager::BroadcastCheckAction()
{
	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_CheckAction), S2C_CHECK_ACTION, nullptr), sizeof(PacketS2C_CheckAction));
}

void ServerNetworkManager::CheckRestartState(std::shared_ptr<Session> session, char* pMsg)
{
	PacketC2S_RestartGame* pRestart = reinterpret_cast<PacketC2S_RestartGame*>(pMsg);
	assert(pRestart != nullptr);

	PacketS2C_RestartGame rg;

	// 메인화면으로 버튼 눌렀을 때
	if (pRestart->restart == '0')
	{
		rg.restart = '0';
		BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_RestartGame), S2C_RESTART_GAME, &rg.restart), sizeof(PacketS2C_RestartGame));
		session->SetRestart(false, true);
		return;
	}
	// 다시 시작 눌렀을 때
	else if (pRestart->restart == '1')
	{
		session->SetRestart(true, true);
	}

	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			// 서버에 안 들어온게 있다면
			if (!pSession->GetReStart().bserverEnter) return;
		}
	}

	// 다 들어왔는지 체크 됐으면
	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			if (!pSession->GetReStart().restart)
			{
				rg.restart = '0';
				BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_RestartGame), S2C_RESTART_GAME, &rg.restart), sizeof(PacketS2C_RestartGame));
				return;
			}
		}
	}
	rg.restart = '1';
	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_RestartGame), S2C_RESTART_GAME, &rg.restart), sizeof(PacketS2C_RestartGame));

	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
		if (pClient == nullptr)	return;
		else
		{
			pSession->SetRestart(false, false);
		}
	}
}

void ServerNetworkManager::IsAllEnter()
{
	if (m_peerClients.size() <= 1)
		return;

	for (auto& pClient : m_peerClients)
	{
		if (pClient == nullptr)
		{
			continue;
		}
		else
		{
			std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
			std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];

			if (pSession == nullptr) return;
			if (!pSession->GetEnterState()) return;
		}
	}

	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_IsAllEnter), S2C_IS_ALL_ENTER, nullptr), sizeof(PacketS2C_IsAllEnter));

	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];

		if (pSession == nullptr) return;

		pSession->SetEnterState(false);
	}
}

// 모두가 Ready인지 체크
void ServerNetworkManager::IsAllReady()
{
	if (m_peerClients.size() <= 1)
		return;

	for (auto& pClient : m_peerClients)
	{
		if (pClient == nullptr)
		{
			continue;
		}
		else
		{
			std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
			std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];

			if (pSession == nullptr) return;
			if (!pSession->GetReadyState())	return;
		}
	}

	BroadcastPacket(SerializeBuffer(sizeof(PacketS2C_IsAllReady), S2C_IS_ALL_READY, nullptr), sizeof(PacketS2C_IsAllReady));

	for (auto& pClient : m_peerClients)
	{
		std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
		std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];

		if (pSession == nullptr) return;

		pSession->SetReadyState(false);
	}
}

void ServerNetworkManager::ServerLoop()
{
	while (m_bRun)
	{
		Update();
		NetUpdate();
	}
}