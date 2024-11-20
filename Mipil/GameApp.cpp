#include "pch.h"
#include "GameApp.h"

#include <directxtk/SimpleMath.h>
#include <shellapi.h>

#include "TestWorld.h"
#include "TestWorld2.h"
#include "TestWorld3.h"
#include "MainWorld.h"
#include "GameOverWorld.h"
#include "Stage1World.h"
#include "Stage2World.h"
#include "Stage3World.h"
#include "Stage4World.h"
#include "Stage5World.h"

#include "StartingCutScene.h"
#include "StartFreeWorld.h"
#include "EndingCutScene.h"
#include "EndingFreeWorld.h"
#include "CreditScene.h"

#include "../Engine/Model.h"
#include "../Engine/GameObject.h"
#include "../Engine/InputManager.h"
#include "../Engine/EventManager.h"
#include "../Engine/NetworkManager.h"

#ifdef _DEBUG
#include <vld.h>
#endif

#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")

#ifdef ENGINE_DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

using namespace DirectX;
using namespace DirectX::SimpleMath;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// todo 채원: 여기서 IP setting 해주던걸 중개서버로 아예 세팅해주기.
	// 처음 시작할 때부터 연결 시키기?


	// 전역 문자열을 초기화합니다.
	GameApp App(hInstance);

	LPWSTR commandLine = GetCommandLineW();

	// 명령행 파싱
	int argc;
	LPWSTR* argv = CommandLineToArgvW(commandLine, &argc);

	if (argc <= 1)
	{
		printf("실행파라메터 개수가 부족함\n");
		App.SetUseNetwork(false);
		// 릴레이 서버 사용하는거 bool 값으로 가지고 있기.
	}
	else if (argc == 2)	// 1 ip
	{
		printf("Guest\n");
		int requiredSize = WideCharToMultiByte(CP_UTF8, 0, argv[1], -1, nullptr, 0, nullptr, nullptr);
		char* utf8String = new char[requiredSize];
		WideCharToMultiByte(CP_UTF8, 0, argv[1], -1, utf8String, requiredSize, nullptr, nullptr);

		App.SetIP(utf8String);
		App.SetUseNetwork(true);
		delete[] utf8String;
	}
	else if (argc == 3)// 1 host 2 ip
	{
		printf("Host\n");
		App.SetHost(true);

		int requiredSize = WideCharToMultiByte(CP_UTF8, 0, argv[2], -1, nullptr, 0, nullptr, nullptr);
		char* utf8String = new char[requiredSize];
		WideCharToMultiByte(CP_UTF8, 0, argv[2], -1, utf8String, requiredSize, nullptr, nullptr);

		App.SetUseNetwork(true);
		App.SetIP(utf8String);
		delete[] utf8String;
	}
	// 메모리 해제
	LocalFree(argv);

	App.Initialize();
	App.Run();
	App.Finalize();

	return (int)1;
}

GameApp::GameApp(HINSTANCE hInstance)
	: CommonApp(hInstance)
{
}

bool GameApp::Initialize()
{
	bool res = __super::Initialize();

	//std::shared_ptr<TestWorld> testWorld = WorldManager::GetInstance()->RegisterWorld<TestWorld>("testWorld", eWorldType::TEST).lock();		// ERROR : 테스트 월드를 안만들면 월드 이동에서 에러 남
	//std::shared_ptr<TestWorld2> testWorld2 = WorldManager::GetInstance()->RegisterWorld<TestWorld2>("testWorld2", eWorldType::TEST2).lock();
	//std::shared_ptr<TestWorld3> testWorld3 = WorldManager::GetInstance()->RegisterWorld<TestWorld3>("testWorld3", eWorldType::TEST3).lock();
	std::shared_ptr<GameOverWorld> gameOverWorld = WorldManager::GetInstance()->RegisterWorld<GameOverWorld>("GameOverWorld", eWorldType::GAMEOVERWORLD).lock();
	std::shared_ptr<MainWorld> mainWorld = WorldManager::GetInstance()->RegisterWorld<MainWorld>("mainWorld", eWorldType::MAIN).lock();
	std::shared_ptr<Stage1World> stage1 = WorldManager::GetInstance()->RegisterWorld<Stage1World>("stage1", eWorldType::STAGE1).lock();
	std::shared_ptr<Stage2World> stage2 = WorldManager::GetInstance()->RegisterWorld<Stage2World>("stage2", eWorldType::STAGE2).lock();
	std::shared_ptr<Stage3World> stage3 = WorldManager::GetInstance()->RegisterWorld<Stage3World>("stage3", eWorldType::STAGE3).lock();
	std::shared_ptr<Stage4World> stage4 = WorldManager::GetInstance()->RegisterWorld<Stage4World>("stage4", eWorldType::STAGE4).lock();
	std::shared_ptr<Stage5World> stage5 = WorldManager::GetInstance()->RegisterWorld<Stage5World>("stage5", eWorldType::STAGE5).lock();

	std::shared_ptr<StartingCutScene> startingCutScene = WorldManager::GetInstance()->
		RegisterWorld<StartingCutScene>("startingCutScene", eWorldType::STARTCUTSCECNE ).lock();
	std::shared_ptr<StartFreeWorld> startFreeWorld = WorldManager::GetInstance()->
		RegisterWorld<StartFreeWorld>("startFreeWorld", eWorldType::STARTFREEWORLD).lock();
	std::shared_ptr<EndingCutScene> endingCutScene = WorldManager::GetInstance()->
		RegisterWorld<EndingCutScene>("endingCutScene", eWorldType::ENDCUTSCENE).lock();
	std::shared_ptr<EndingFreeWorld> endingFreeWorld = WorldManager::GetInstance()->
		RegisterWorld<EndingFreeWorld>("endingFreeWorld", eWorldType::ENDFREEWORLD).lock();
	std::shared_ptr<CreditScene> creditScene =  WorldManager::GetInstance()->
		RegisterWorld<CreditScene>("creditScene", eWorldType::CREDIT).lock();

	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, startingCutScene.get());
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, startFreeWorld.get());
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, endingCutScene.get());
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, endingFreeWorld.get());
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, gameOverWorld.get());
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, creditScene.get());
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, gameOverWorld.get());

	//WorldManager::GetInstance()->SetDefaultWorld(testWorld3);
	//WorldManager::GetInstance()->SetDefaultWorld(stage1);
	//WorldManager::GetInstance()->SetDefaultWorld(stage2);
	//WorldManager::GetInstance()->SetDefaultWorld(stage3);
	//WorldManager::GetInstance()->SetDefaultWorld(stage4);
	WorldManager::GetInstance()->SetDefaultWorld(mainWorld);
	//WorldManager::GetInstance()->SetDefaultWorld(gameOverWorld);
	//WorldManager::GetInstance()->SetDefaultWorld(testWorld);
	//WorldManager::GetInstance()->SetDefaultWorld(startFreeWorld);
	//WorldManager::GetInstance()->SetDefaultWorld(startingCutScene);
	//WorldManager::GetInstance()->SetDefaultWorld(endingFreeWorld);
	//WorldManager::GetInstance()->SetDefaultWorld(creditScene);
	//WorldManager::GetInstance()->SetDefaultWorld(creditScene);


	WorldManager::GetInstance()->Initialize();

	/// 이벤트 매니저는 제일 끝에
	EventManager::GetInstance()->Initialize();

	return res;
}

void GameApp::Update()
{
	WorldManager::GetInstance()->Update(m_deltaTime);

	CommonApp::Update();
}

void GameApp::Render()
{
	__super::Render();

	/// 이벤트 매니저는 제일 끝에
	EventManager::GetInstance()->Run();
}

void GameApp::Finalize()
{
	CommonApp::Finalize();
}

LRESULT GameApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::WndProc(hWnd, message, wParam, lParam);
}
