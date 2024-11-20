#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <memory>
#include <thread>

#include "TimeManager.h"

#include "../D3DRenderer/D3DRenderManager.h"
#include "../Engine/Server.h"
#include "../Engine/Client.h"

class D3DRenderManager;
class ResourceManager;

const int MAX_LOADSTRING = 100;

/// <summary>
/// 게임에서 사용할 공통적인 처리를 하는 class
/// 실제로 만들 게임은 CommonApp을 상속 받아 부가적인 처리를 한다.
///	싱글톤 패턴으로 만들어 Window, DirectX 자원에 대한 접근을 용이하게 한다.
/// </summary>
class CommonApp
{
protected:
	CommonApp(HINSTANCE hInstance);
	virtual ~CommonApp() = default;

public:
	static HWND GetHwnd() { return m_hWnd; }
	std::shared_ptr<D3DRenderManager> GetRenderer() const { return m_pD3DRenderer; }
	ComPtr<ID3D11Device> GetDevice() const { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDevcon() const { return m_pDeviceContext; }


public:
	virtual bool Initialize();
	void Run();
	void GameLoop();
	virtual void Update();
	virtual void Render();
	virtual void Finalize();

	void Exit() { m_bRun = false; }

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	void ShowFrameRate();


public:
	static CommonApp* m_pInstance;	// 매 프레임 돌아야하기 때문에 public으로 선언

protected:
	static HWND m_hWnd;

	HACCEL m_hAccelTable;
	MSG m_msg;
	HINSTANCE m_hInstance;
	WCHAR m_szTitle[MAX_LOADSTRING];
	WCHAR m_szWindowClass[MAX_LOADSTRING];
	WNDCLASSEXW m_wcex;
	int m_nCmdShow;

	FLOAT m_deltaTime;

/// Manager
	std::unique_ptr<TimeManager> m_pTimeManager;

// Network
	ServerNetworkManager m_server;
	ClientNetworkManager m_client;

	// 현재 호스트인지 아닌지
	bool m_bHost = false;
	// 내 아이피
	std::string m_ip;
	// 네트워크 쓰는지 안 쓰는지
	bool m_bUseNetwork = false;
	bool m_bRun = true;

/// D3DRenderer와 연결된 자원
protected:
	std::shared_ptr<D3DRenderManager> m_pD3DRenderer = nullptr;

	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;
};

