#pragma once
#include "CircularQueue.h"
#include "../NetworkLibrary/MyProtocol.h"

/// <summary>
/// 클라이언트와 서버에서 받은 데이터를 게임로직에 전달하는 곳.(반대로도)
/// 프로토콜 해석하는 곳.
/// 버퍼(큐)는 여기서 관리
/// </summary>

class NetworkManager
{
public:
	void Initialize();
	void Finalize();
};
