#pragma once

#include "../Engine/Client.h"
#include  "../Engine/CommonApp.h"
#include "../Engine/Server.h"
#include "../Engine/WorldManager.h"

#include <memory>
#include <thread>

class WorldManager;
class Model;
class TestWorld;

/// <summary>
///	실제 구동되는 게임 애플리케이션
/// 매니저들을 Init, Run, Render한다.
/// </summary>
class GameApp
	: public CommonApp
{
public:
	GameApp(HINSTANCE hInstance);
	~GameApp() = default;

public:
	// __suer::init 이전에 셰이더 파일 이름 지정해야함
	bool Initialize() final;
	void Update() final;
	void Render() final;
	void Finalize() final;

	void SetHost(bool isHost) { m_bHost = isHost	;}
	void SetIP(std::string ip) { m_ip = ip; }
	void SetUseNetwork(bool useNetwork) { m_bUseNetwork = useNetwork; }

private:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

