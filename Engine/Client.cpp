#include "pch.h"
#include "Client.h"
#include "WorldManager.h"

#include "../NetworkLibrary/ClientSocket.h"
#include "../NetworkLibrary/WinSock.h"

void ClientNetworkManager::Start(const char* add)
{
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return;

    m_clientSocket = std::make_shared<ClientSocket>();
    if (!m_clientSocket->Create())
        return;

    m_connectResult = m_clientSocket->Connect(add, 7777);
    m_address = add;

    m_hQuitEvent = CreateEvent(NULL, false, false, NULL);
    m_bRun = true;
}

void ClientNetworkManager::Update()
{
    if (WAIT_TIMEOUT != WaitForSingleObject(m_hQuitEvent, 0))
    {
        m_bRun = false;
        return;
    }

    WSAEVENT clientEvent = m_clientSocket->GetEvent();
    int index = ::WSAWaitForMultipleEvents(1, &clientEvent, FALSE, 1, FALSE);

    if (index == WSA_WAIT_FAILED)
        return;

    index -= WSA_WAIT_EVENT_0;

    if (index == WSA_WAIT_TIMEOUT)
        return;

    WSANETWORKEVENTS networkEvents;
    if (::WSAEnumNetworkEvents(m_clientSocket->GetHandle(), clientEvent, &networkEvents) == SOCKET_ERROR)
        return;

    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
        {
            OnNetError(networkEvents.iErrorCode[FD_CONNECT_BIT], "Connect", m_clientSocket);
            return;
        }
        return;
    }

    if (networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            OnNetError(networkEvents.iErrorCode[FD_READ_BIT], "Read", m_clientSocket);
            return;
        }
        OnReceive();
    }

    if (networkEvents.lNetworkEvents & FD_WRITE)
    {
        if (networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
        {
            OnNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Write", m_clientSocket);

            printf("dddd\n");
            return;
        }

        OnSend();
    }

    if (networkEvents.lNetworkEvents & FD_CLOSE)
    {
        if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
        {
            return;
        }
    }
}

void ClientNetworkManager::NetUpdate()
{
    while(!WorldManager::GetInstance()->IsSendQueueEmpty())
    {
        std::pair<char*, int> value;

        if (!WorldManager::GetInstance()->PopSendQueue(value))
            break;

        int nSent = m_clientSocket->Send(value.first, value.second);

        delete[] value.first;
    }
}

void ClientNetworkManager::OnReceive()
{
    int recvBytes = m_clientSocket->Recv(m_recvBuffer, m_recvBytes);

    if (recvBytes == SOCKET_ERROR)
    {
        return;
    }
    // 읽을거 없을 때
    if (recvBytes == 0)
    {
        return;
    }
    
    m_recvBytes += recvBytes;

    Read(m_recvBuffer);
}

void ClientNetworkManager::OnSend()
{

}

void ClientNetworkManager::OnClose()
{
}

void ClientNetworkManager::Stop()
{
    if (m_hQuitEvent != INVALID_HANDLE_VALUE)
        SetEvent(m_hQuitEvent);
}

void ClientNetworkManager::CleanUp()
{
    if (m_hQuitEvent != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_hQuitEvent);
    }

    ::WSACleanup();
}

void ClientNetworkManager::Read(char* pData)
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

        WorldManager::GetInstance()->PushRecvQueue(packet, sizeInfo);
    }
}

void ClientNetworkManager::Write(char* buf, int len)
{
}

void ClientNetworkManager::OnNetError(int errorCode, const char* errorMsg, std::shared_ptr<WinSock> pSocket)
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

void ClientNetworkManager::ClientLoop()
{
    while (m_bRun)
    {
        Update();
        NetUpdate();
    }
}