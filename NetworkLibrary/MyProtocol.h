#pragma once
#include <cstddef>
#include <string>

#pragma pack (push, 1)

#define MAX_LEN 100

enum EPacketId : short
{
	PACKETID_START,

	C2S_BROADCAST_MSG,
	S2C_BROADCAST_MSG,

	// 서버에게 내가 host라고 전달해주기(그러면 Session에서 m_bHost를 true로 만들어줌)
	C2S_IAM_HOST_MSG,

	// 클라가 레디 버튼 눌렀을 때
	C2S_READY,

	// 모든 클라가 스타트자유이동 목표지점에 도착했는지
	C2S_IS_ALL_GOAL,
	S2C_IS_ALL_GOAL,

	// 모든 클라가 엔딩자유이동 목표지점에 도착했는지
	C2S_IS_ALL_END,
	S2C_IS_ALL_END,

	// 모든 클라가 스테이지에 들어왔는지
	C2S_IS_ALL_ENTER,
	S2C_IS_ALL_ENTER,

	// 모든 클라가 레디인지 확인
	C2S_IS_ALL_READY,
	S2C_IS_ALL_READY,

	// 자신의 행동들을 브로드캐스트
	C2S_BROADCAST_ACTION,
	S2C_BROADCAST_ACTION,

	// 누구턴인지 턴 세팅해주기
	C2S_SET_TURN,
	S2C_SET_TURN,

	// 턴 시작이다
	C2S_START_TURN,
	S2C_START_TURN,

	// 턴 바꾸기
	C2S_CHANGE_TURN,
	S2C_CHANGE_TURN,

	// 행동이 남아있는지 확인
	C2S_CHECK_ACTION,
	S2C_CHECK_ACTION,

	// 모든 행동이 끝났다.
	C2S_END_ACTION,
	S2C_END_ACTION,

	// 움직임이 끝났다.
	C2S_MOVEMENT_END,
	S2C_MOVEMENT_END,

	// 다시 시작
	C2S_RESTART_GAME,
	S2C_RESTART_GAME,

	// 캐릭터가 어디로 이동하는지 Broadcast
	S2C_BROADCAST_MOVE,

	PACKETID_END,
};

struct PacketHeader
{
	short size;
	EPacketId id;
};

struct PacketC2S_Access : PacketHeader
{
	char nickname[MAX_LEN];
};

struct PacketS2C_Access : PacketHeader
{
	char result;
};

struct PacketC2S_BroadcastMsg : PacketHeader
{
	char clientMessage[MAX_LEN];
};

struct PacketS2C_BroadcastMsg : PacketHeader
{
	char serverMessage[MAX_LEN];
};

// 세션에게 호스트인지 알려주기 위해서
struct PacketC2S_IamHost : PacketHeader
{
	char host;
};
// 클라에서 레디 버튼 눌렀을 때
struct PacketC2S_READY : PacketHeader
{
	char clickedReady;
};

struct PacketC2S_IsAllEnd : PacketHeader
{
};

struct PacketS2C_IsAllEnd : PacketHeader
{
};

struct PacketC2S_IsAllGoal : PacketHeader
{
};

struct PacketS2C_IsAllGoal : PacketHeader
{
};

struct PacketC2S_IsAllEnter : PacketHeader
{
};
// 
struct PacketS2C_IsAllEnter : PacketHeader
{
};

// 클라에서 게임 시작 전 모든 클라가 레디상태인지 요청
struct PacketC2S_IsAllReady : PacketHeader
{
};
// 서버가 클라한테 모두 레디면 true 아니면 false 보내준다.
struct PacketS2C_IsAllReady : PacketHeader
{
};

// 클라에서 서버한테 어떤 행동인지 알려주기
struct PacketC2S_BroadcastAction : PacketHeader
{
	char who;
	char action[4];
};

// 서버에서 어떤 클라가 어떤 행동인지 브로드캐스트해주기.
struct PacketS2C_BroadcastAction : PacketHeader
{
	char who;
	char action[4];
};

struct PacketC2S_SetTurn : PacketHeader
{
	// '0'이면 host, '1'이면 클라이언트
	char who;
};

struct PacketS2C_SetTurn : PacketHeader
{
	// '0'이면 host, '1'이면 클라이언트
	char setTurn;
};

struct PacketC2S_StartTurn : PacketHeader
{
	// 턴 시작하기
};

struct PacketC2S_EndTurn : PacketHeader
{
	// 호스트가 종료 요청했는지, 게스트가 종료 요청했는지
};

struct PacketS2C_EndTurn : PacketHeader
{
	// 행동이 끝나면 턴 종료하기(client에 있는 turn 불값을 false로 바꿔준다.
};

struct PacketC2S_ChangeTurn : PacketHeader
{
	// 호스트가 종료 요청했는지, 게스트가 종료 요청했는지
};

struct PacketS2C_ChangeTurn : PacketHeader
{
	// 행동이 끝나면 턴 종료하기(client에 있는 turn 불값을 false로 바꿔준다.
};

struct PacketS2C_StartGame : PacketHeader
{
	// 게임을 시작
};

struct PacketC2S_CheckAction : PacketHeader
{
	// 행동이 남아있는지 확인
};

struct PacketS2C_CheckAction : PacketHeader
{
	// 행동이 남아있는지 확인
};

struct PacketC2S_EndAction : PacketHeader
{
	// 게임을 끝
};

struct PacketS2C_EndAction : PacketHeader
{
	// 게임을 끝
};

struct PacketC2S_MovementEnd : PacketHeader
{
	// 움직임이 끝났다
	char who;
};

struct PacketS2C_MovementEnd : PacketHeader
{
	// 움직임이 끝났다.
};

struct PacketC2S_RestartGame : PacketHeader
{
	// 게임 다시 시작
	char restart; // 0이면 다시 시작 안 함, 1이면 다시 시작
};

struct PacketS2C_RestartGame : PacketHeader
{
	// 게임 다시 시작
	char restart; // 0이면 다시 시작 안 함, 1이면 다시 시작
};

#pragma pack (pop)