#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment (lib, "Ws2_32.lib")

/// <summary>
/// 소켓의 최상위단
/// 소켓의 이벤트를 이곳에서 관리해준다.
/// </summary>

class WinSock
{
public:
	WinSock();
	virtual ~WinSock();

	virtual bool OnAccept(std::shared_ptr<WinSock> pSocket) { return false; }
	virtual bool OnConnect() { return false; };
	virtual bool OnClose() { return false; };
	virtual bool OnSend() { return false; };
	virtual bool OnReceive() { return false; };

	bool Create(
		int nSocketType = SOCK_STREAM,
		long lEvent = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE
	);

	bool Bind(int port, const char* lpszSocketAddress = nullptr);

	bool Listen(int backlog = SOMAXCONN);

	bool Accept(WinSock& acceptSocket);

	int Connect(const char* ip, int port);

	bool DisConnect();

	void Close();

	int Send(char* buf, int len);
					 
	int Recv(char* buf, int len);

	int SendTo(const char* buf, int len, const char* ip, int port);

	int RecvFrom(char* buf, int len, char* ip, int* port);

	SOCKET GetHandle() { return m_hSocket; }
	WSAEVENT GetEvent() { return m_hEvent; }

	void SetEvent(WSAEVENT hEvent) { m_hEvent = hEvent; }

	void Attach(SOCKET hSocket) { m_hSocket = hSocket; }
	void Detach() { m_hSocket = INVALID_SOCKET; }

	const std::string& GetIP() { return m_ip; }
	int GetPort() { return m_port; }

	int GetLastError() { return m_error; }

private:
	SOCKET		m_hSocket = INVALID_SOCKET;
	WSAEVENT	m_hEvent = WSA_INVALID_EVENT;

	std::string m_ip;
	int			m_port = 0;

	int m_error;
};
