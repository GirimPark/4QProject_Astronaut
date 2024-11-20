#pragma once
#include "WinSock.h"

/// <summary>
/// ClientNetworkManager에서 사용하는 소켓
/// WinSock을 상속받아 현재 소켓이 어떤 이벤트 상태인지 확인한 후에 
/// 그에 알맞는 함수 호출.
/// </summary>
class ClientSocket : public WinSock
{
public:
	ClientSocket() = default;
	~ClientSocket() = default;

	virtual bool OnAccept(std::shared_ptr<WinSock> pSocket) override { return false; }
	virtual bool OnConnect() override { return false; }
	virtual bool OnClose() override { return false; }
	virtual bool OnSend() override { return false; }
	virtual bool OnReceive() override { return false; }
};
