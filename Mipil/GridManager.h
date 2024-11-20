#pragma once

class TestPlayerObject;
class PlayerObject;
class GameObject;
enum class eObjectType;

enum class eGridType
{
	EMPTY,
	BLOCK,
	PLAYER,
	PART,

	END
};

struct GridPos
{
	int x;
	int y;
	int z;

	GridPos& operator+(const GridPos& other) const
	{
		GridPos rt = { x + other.x, y + other.y, z + other.z };
		return rt;
	}

	GridPos& operator-(const GridPos& other) const
	{
		GridPos rt = { x - other.x, y - other.y, z - other.z };
		return rt;
	}

	bool operator==(const GridPos& other) const
	{
		if (x == other.x && y == other.y && z == other.z)
			return true;
		return false;
	}

	GridPos& operator+(const Vector3& other) const
	{
		GridPos rt = { x + static_cast<int>(other.x), y + static_cast<int>(other.y), z + static_cast<int>(other.z) };
		return rt;
	}

	GridPos& operator-(const Vector3& other) const
	{
		GridPos rt = { x - static_cast<int>(other.x), y - static_cast<int>(other.y), z - static_cast<int>(other.z) };
		return rt;
	}
};

enum class eMinimapType
{
	EMPTY,	// 목적지 또한 empty에 해당. 미니맵은 "부품"과 플레이어 위치만 표시하기 때문
	HOST,
	GUEST,
	OVERLAP,
	PART,

	END
};

struct MinimapPos
{
	int x;
	int z;
};

/// <summary>
///	CreateMap을 통해 처음 맵의 규격을 정한다.
/// 그리드 맵에서 동작할 오브젝트들은 SetMapState를 통해 맵 정보를 쓴다.
/// 그리드 맵은 아래평면 좌 하단 기준으로 배치된다.
/// DX 좌표계 기준으로 맵의 row는 Z축, col은 X축이다.
/// </summary>
class GridManager
{
public:
	GridManager() = default;
	~GridManager() = default;

	// 처음 맵, 그리드 크기, 시작 중점 위치 설정
	void CreateMap(int col, int height, int row, float startXPos, float startYPos, float startZPos, float gridSize);

	// 오브젝트로부터 위치 읽어서 그리드 겹침 확인 후 맵에 적용
	void SetMapState(std::shared_ptr<GameObject> object); 
	void SetMapState(std::vector<std::shared_ptr<GameObject>>& objects);

	// 인자로 이동 타입 받은 후, 가능성 확인 및 이동처리. 그리드 맵 수정
	//void MoveOnGrid(TestPlayerObject* player, Vector3 vec);	// 테스트용
	void MoveOnGrid(PlayerObject* player, Vector3 vec);

	//void FallToGround(TestPlayerObject* player);	// 테스트용
	void FallToGround(PlayerObject* player);	// 테스트용

	bool IsGround(Vector3 pos) const;
	bool IsPlayerAbove(Vector3 pos) const;

	int GetNumCol() const { return m_numCol; }
	int GetNumRow() const { return m_numRow; }

	std::vector<std::pair<MinimapPos, eMinimapType>>& GetMinimapInfo();
	void ClearMinimapInfo();

	GridPos& ConvertToGridPos(Vector3 position) const;	// vector3 좌표 그리드 좌표로 변환

private:
	Vector3& ConvertToDXPos(GridPos position) const;

	bool CheckDuplicated(GridPos& gridPos) const;
	eGridType MatchingGridType(eObjectType type) const;

	bool CheckMovePossibility(GridPos& gridPos, Vector3 vec);	// 이동 가능 여부와 방향 리턴

	int CalculateIdx(int x, int z) const { return z * m_numCol + x; }


	int m_numCol;		// x
	int m_numHeight;	// y
	int m_numRow;		// z
	float m_startXPos;	// 블럭의 중점이 아닌 끝점으로 CreateMap에서 조정한다.
	float m_startYPos;
	float m_startZPos;
	float m_gridSize;

	int m_fallCnt;
	bool m_bGetPart = false;

	std::vector<std::vector<eGridType>> m_gridMap;

	std::vector<std::vector<eMinimapType>> m_miniMap;
	std::vector<std::pair<MinimapPos, eMinimapType>> m_miniMapInfos;
};