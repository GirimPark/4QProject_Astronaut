#include "pch.h"
#include "NetworkManager.h"

#include <winsock2.h>
#include <ws2tcpip.h>

void NetworkManager::Initialize()
{
	WSAData wsaData;

	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return;
}

void NetworkManager::Finalize()
{
	::WSACleanup();
}
