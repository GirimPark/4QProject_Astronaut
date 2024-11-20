#include "pch.h"
#include "Session.h"

#include "Client.h"

#include "../NetworkLibrary/ClientSocket.h"
#include "../NetworkLibrary/MyProtocol.h"
#include <iostream>

SessionId Session::s_sessionIdCounter = 0;

Session::Session()
{
}

Session::~Session()
{
}

void Session::Write(char* pData, int nSent)
{
}

void Session::Read(char* pData)
{
	// 온전한 패킷을 recvQueue에 넣는다.
	while (m_recvBytes > 0)
	{
		// 온전한 패킷인지 확인
		short sizeInfo;
		memcpy(&sizeInfo, pData, sizeof(short));
		if (m_recvBytes < sizeInfo)
			break;

		// 온전한 패킷인 경우
		char* packet = new char[sizeInfo + 1];
		memcpy(packet, pData, sizeInfo);
		m_recvBytes -= sizeInfo;
		memmove(m_recvBuffer, m_recvBuffer + sizeInfo, RCV_BUF_SIZE - sizeInfo);

		m_recvQueue.push({ packet, sizeInfo });
	}
}

void Session::ReadUpdate()
{
	std::shared_ptr<ClientSocket> soc = m_pPeerClient->GetSocket();
	int nRead = soc->Recv(m_recvBuffer, m_recvBytes);

	if (nRead == -1)
	{
		return;
	}
	m_recvBytes += nRead;

	Read(m_recvBuffer);
}

void Session::SetClient(std::shared_ptr<ClientNetworkManager> pClient)
{
	m_sessionId = s_sessionIdCounter++;

	m_pPeerClient = pClient;

	m_pPeerClient->SetSessionId(m_sessionId);
}

void Session::SetMovementFalse()
{
	for (short i = 0; i < 2; i++)
		m_movement[i] = false;
}

bool Session::GetAllMovementEnd()
{
	for (short i = 0; i < 2; i++)
	{
		if (!m_movement[i]) 
			return false;
	}
	return true;
}

int Session::Send(char* packet, int len)
{
	std::shared_ptr<ClientSocket> soc = m_pPeerClient->GetSocket();

	int nSent = soc->Send(packet, len);

	return nSent;
}

void Session::Finalize()
{
	while (!m_recvQueue.empty())
	{
		std::pair<char*, int> value;

		if (m_recvQueue.try_pop(value))
			delete[] value.first;
	}

	while (!m_sendQueue.empty())
	{
		std::pair<char*, int> value;
		if (m_sendQueue.try_pop(value))
			delete[] value.first;
	}
}
