#include "pch.h"
#include "framework.h"

#include "WinSock.h"

WinSock::WinSock()
{
}

WinSock::~WinSock()
{
    WSACloseEvent(m_hEvent);
    Close();
}

bool WinSock::Create(int nSocketType, long lEvent)
{
    m_hSocket = ::WSASocket(AF_INET, nSocketType, 0, nullptr, 0, 0);

    if (m_hSocket == INVALID_SOCKET)
    {
        m_error = ::WSAGetLastError();
        return false;
    }

    m_hEvent = ::WSACreateEvent();
    if (m_hEvent == WSA_INVALID_EVENT) return false;

    if (::WSAEventSelect(m_hSocket, m_hEvent, lEvent) == SOCKET_ERROR)
    {
        m_error = ::WSAGetLastError();
        return false;
    }

    return true;
}

bool WinSock::Bind(int port, const char* lpszSocketAddress)
{
    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;

    ::inet_pton(AF_INET, lpszSocketAddress, &(serverAddr.sin_addr));

    serverAddr.sin_port = ::htons(port);

    if (::bind(m_hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        m_error = ::WSAGetLastError();
        return false;
    }

    m_ip = lpszSocketAddress;
    m_port = port;

    return true;
}

bool WinSock::Listen(int nConnectionBacklog)
{
    if (::listen(m_hSocket, nConnectionBacklog) == SOCKET_ERROR)
    {
        m_error = ::WSAGetLastError();
        return false;
    }
    return true;
}

bool WinSock::Accept(WinSock& acceptSocket)
{
    SOCKADDR_IN clientAddr;
    int addrLen = sizeof(clientAddr);

    SOCKET hClientSocket = ::accept(m_hSocket, (SOCKADDR*)&clientAddr, &addrLen);

    if (hClientSocket == INVALID_SOCKET)
    {
        m_error = ::WSAGetLastError();
        return false;
    }

    acceptSocket.Attach(hClientSocket);

    char ipv4AddressAsString[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), ipv4AddressAsString, INET_ADDRSTRLEN);
    acceptSocket.m_ip = ipv4AddressAsString;

    acceptSocket.m_port = ::ntohs(clientAddr.sin_port);

    WSAEVENT clientEvent = ::WSACreateEvent();
    acceptSocket.SetEvent(clientEvent);

    if (::WSAEventSelect(hClientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
    {
        m_error = ::WSAGetLastError();

        WSACloseEvent(clientEvent);

        closesocket(hClientSocket);

        return false;
    }

    return true;
}

int WinSock::Connect(const char* ip, int port)
{
    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    // todo: 포트번호
    serverAddr.sin_port = ::htons(port);
    inet_pton(AF_INET, ip, &(serverAddr.sin_addr));

    int connectedResult = ::connect(m_hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    return connectedResult;
}

bool WinSock::DisConnect()
{
    int iResult = shutdown(m_hSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
        return false;
    return true;
}

void WinSock::Close()
{
    ::closesocket(m_hSocket);
}

int WinSock::Send(char* buf, int len)
{
    int sendLen = ::send(m_hSocket, buf, len, 0);

    if (sendLen > 0)
    {
        buf[sendLen] = '\0';
    }


    m_error = ::WSAGetLastError();

    if (sendLen == SOCKET_ERROR && m_error != WSAEWOULDBLOCK)
    {
        return SOCKET_ERROR;
    }

    return sendLen;
}

int WinSock::Recv(char* buf, int len)
{
    int recvLen = ::recv(m_hSocket, buf + len, RCV_BUF_SIZE - len, 0);

    if (recvLen > 0)
    {
        buf[recvLen] = '\0';
    }

    m_error = ::WSAGetLastError();

    if (recvLen == SOCKET_ERROR && m_error != WSAEWOULDBLOCK)
    {
        return SOCKET_ERROR;
    }
    return recvLen;
}

int WinSock::SendTo(const char* buf, int len, const char* ip, int port)
{
    return 0;
}

int WinSock::RecvFrom(char* buf, int len, char* ip, int* port)
{
    return 0;
}
