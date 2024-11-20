#include "pch.h"
#include "GridManager.h"

//#include <ios>

#include "PlayerObject.h"
#include "TestPlayerObject.h"

#include "../Engine/EventManager.h"
//#include "Stage1World.h"

void GridManager::CreateMap(int col, int height, int row, float startXPos, float startYPos, float startZPos, float gridSize)
{
	m_numCol = col;
	m_numHeight = height;
	m_numRow = row;
	m_startXPos = startXPos;
	m_startYPos = startYPos;
	m_startZPos = startZPos;
	m_gridSize = gridSize;

	m_gridMap.resize(m_numHeight, std::vector<eGridType>(m_numRow * m_numCol, eGridType::EMPTY));
	m_miniMap.resize(col);
	for(auto& z : m_miniMap)
	{
		z.resize(row);
	}
}

void GridManager::SetMapState(std::shared_ptr<GameObject> object)
{
	GridPos gridPos = ConvertToGridPos(object->GetLocalPosition());

	if(CheckDuplicated(gridPos))
	{
		LOG_CONTENTS("해당 그리드에 이미 데이터가 있으므로 생략");
	}
	else
	{
		m_gridMap[gridPos.y][CalculateIdx(gridPos.x, gridPos.z)] = MatchingGridType(object->GetObjectType());
	}
}

void GridManager::SetMapState(std::vector<std::shared_ptr<GameObject>>& objects)
{
	for(const auto& object : objects)
	{
		GridPos gridPos = ConvertToGridPos(object->GetLocalPosition());

		if (CheckDuplicated(gridPos))
		{
			LOG_CONTENTS("해당 그리드에 이미 데이터가 있으므로 생략");
		}
		else
		{
			eGridType gridType = MatchingGridType(object->GetObjectType());
			m_gridMap[gridPos.y][CalculateIdx(gridPos.x, gridPos.z)] = gridType;

			// 미니맵 세팅
			if (gridType == eGridType::PART)
			{
				m_miniMap[gridPos.x][gridPos.z] = eMinimapType::PART;
			}
			if(gridType == eGridType::PLAYER)
			{
				PlayerObject* player = dynamic_cast<PlayerObject*>(object.get());
				assert(player);

				if(player->GetHostPlayer())
					m_miniMap[gridPos.x][gridPos.z] = eMinimapType::HOST;
				else
					m_miniMap[gridPos.x][gridPos.z] = eMinimapType::GUEST;
			}
		}
	}
}

//void GridManager::MoveOnGrid(TestPlayerObject* player, Vector3 vec)
//{
//	GridPos objPos = ConvertToGridPos(player->GetWorldPosition());
//	GridPos checkPos = objPos;
//
//	if (vec == grid::STAY)
//		return;
//
//	// 합체 상태에서 부모 플레이어의 상승 행동일 경우 자식 플레이어의 위치를 반영한다.
//	if (vec == grid::Y_UP && player->GetComplexType() == eComplexType::PARENT)
//		checkPos.y++;
//
//	if (!CheckMovePossibility(checkPos, vec))
//	{
//		EventManager::GetInstance()->SendEvent(eEventType::CRASH_ON_GRID, player, vec * m_gridSize, true);
//	}
//	else
//	{
//		GridPos newPos = objPos + vec;
//
//		if (player->GetComplexType() == eComplexType::PARENT)
//		{
//			m_gridMap[objPos.y + 1][CalculateIdx(objPos.x, objPos.z)] = eGridType::EMPTY;
//			m_gridMap[newPos.y + 1][CalculateIdx(newPos.x, newPos.z)] = eGridType::PLAYER;
//		}
//
//		m_gridMap[objPos.y][CalculateIdx(objPos.x, objPos.z)] = eGridType::EMPTY;
//		m_gridMap[newPos.y][CalculateIdx(newPos.x, newPos.z)] = eGridType::PLAYER;
//
//		// 부모 오브젝트가 움직였다면, 자식 오브젝트에 대한 움직임도 처리해야 한다.
//
//		EventManager::GetInstance()->SendEvent(eEventType::MOVE_ON_GRID, player, vec * m_gridSize);	// TEST : 즉시 이동 테스트 때문에 gridSize 곱해줌
//	}
//}

void GridManager::MoveOnGrid(PlayerObject* player, Vector3 vec)
{
	GridPos objPos = ConvertToGridPos(player->GetWorldPosition());
	GridPos checkPos = objPos;

	// 합체 상태에서 부모 플레이어의 상승 행동일 경우 자식 플레이어의 위치를 반영한다.
	if (vec == grid::Y_UP && player->GetComplexType() == eComplexType::PARENT)
		checkPos.y++;

	if (!CheckMovePossibility(checkPos, vec))
	{
		EventManager::GetInstance()->SendEvent(eEventType::CRASH_ON_GRID, player, vec * m_gridSize, true);
	}
	else
	{
		GridPos newPos = objPos + vec;

		// 부모 오브젝트가 움직였다면, 자식 오브젝트에 대한 움직임도 처리해야 한다.
		// 상승 행동시 자식을 먼저 움직여야 부모 플레이어가 그리드 상에서 지워지지 않는다.
		if (player->GetComplexType() == eComplexType::PARENT)
		{
			m_gridMap[objPos.y + 1][CalculateIdx(objPos.x, objPos.z)] = eGridType::EMPTY;
			m_gridMap[newPos.y + 1][CalculateIdx(newPos.x, newPos.z)] = eGridType::PLAYER;
		}

		m_gridMap[objPos.y][CalculateIdx(objPos.x, objPos.z)] = eGridType::EMPTY;
		m_gridMap[newPos.y][CalculateIdx(newPos.x, newPos.z)] = eGridType::PLAYER;

		// 미니맵
		m_miniMap[objPos.x][objPos.z] = eMinimapType::EMPTY;
		if (player->GetHostPlayer())
		{
			if (m_miniMap[newPos.x][newPos.z] == eMinimapType::GUEST)
				m_miniMap[newPos.x][newPos.z] = eMinimapType::OVERLAP;
			else
				m_miniMap[newPos.x][newPos.z] = eMinimapType::HOST;
		}
		else
		{
			if (m_miniMap[newPos.x][newPos.z] == eMinimapType::HOST)
				m_miniMap[newPos.x][newPos.z] = eMinimapType::OVERLAP;
			else
				m_miniMap[newPos.x][newPos.z] = eMinimapType::GUEST;
		}

		// overlap인 경우 미니맵 덮어쓰기
		if (player->GetComplexType() == eComplexType::PARENT)
		{
			m_miniMap[newPos.x][newPos.z] = eMinimapType::OVERLAP;
		}

		EventManager::GetInstance()->SendEvent(eEventType::MOVE_ON_GRID, player, vec * m_gridSize, true);	// TEST : 즉시 이동 테스트 때문에 gridSize 곱해줌
	}
}

//void GridManager::FallToGround(TestPlayerObject* player)
//{
//	GridPos playerPos = ConvertToGridPos(player->GetWorldPosition());
//	GridPos goalPos = playerPos;
//
//	if (goalPos.y <= 0)
//		return;
//
//	while (m_gridMap[goalPos.y - 1][CalculateIdx(goalPos.x, goalPos.z)] == eGridType::EMPTY
//		|| m_gridMap[goalPos.y - 1][CalculateIdx(goalPos.x, goalPos.z)] == eGridType::PART)
//	{
//		goalPos.y--;
//
//		EventManager::GetInstance()->SendEvent(eEventType::MOVE_ON_GRID, player, grid::Y_DOWN * m_gridSize);
//
//		if (goalPos.y == 0)
//			break;
//	}
//
//	m_gridMap[playerPos.y][CalculateIdx(playerPos.x, playerPos.z)] = eGridType::EMPTY;
//	m_gridMap[goalPos.y][CalculateIdx(goalPos.x, goalPos.z)] = eGridType::PLAYER;
//
//	if (player->GetComplexType() == eComplexType::PARENT)
//	{
//		m_gridMap[playerPos.y + 1][CalculateIdx(playerPos.x, playerPos.z)] = eGridType::EMPTY;
//		m_gridMap[goalPos.y + 1][CalculateIdx(goalPos.x, goalPos.z)] = eGridType::PLAYER;
//	}
//}

void GridManager::FallToGround(PlayerObject* player)
{
	m_fallCnt = 0;

	GridPos playerPos = ConvertToGridPos(player->GetWorldPosition());
	GridPos goalPos = playerPos;

	if (goalPos.y <= 0)
		return;

	while (m_gridMap[goalPos.y - 1][CalculateIdx(goalPos.x, goalPos.z)] == eGridType::EMPTY
		|| m_gridMap[goalPos.y - 1][CalculateIdx(goalPos.x, goalPos.z)] == eGridType::PART)
	{
		goalPos.y--;
		m_fallCnt++;

		if (goalPos.y == 0)
			break;
	}

	if(m_fallCnt > 0)
		EventManager::GetInstance()->SendEvent(eEventType::MOVE_ON_GRID, player, grid::Y_DOWN * m_gridSize * m_fallCnt, false);

	m_gridMap[playerPos.y][CalculateIdx(playerPos.x, playerPos.z)] = eGridType::EMPTY;
	m_gridMap[goalPos.y][CalculateIdx(goalPos.x, goalPos.z)] = eGridType::PLAYER;

	if (player->GetComplexType() == eComplexType::PARENT)
	{
		m_gridMap[playerPos.y + 1][CalculateIdx(playerPos.x, playerPos.z)] = eGridType::EMPTY;
		m_gridMap[goalPos.y + 1][CalculateIdx(goalPos.x, goalPos.z)] = eGridType::PLAYER;
	}
}

bool GridManager::IsGround(Vector3 pos) const
{
	GridPos gridPos = ConvertToGridPos(pos);
	if (gridPos.y == 0)
		return true;

	if (m_gridMap[gridPos.y - 1][CalculateIdx(gridPos.x, gridPos.z)] == eGridType::BLOCK
		|| m_gridMap[gridPos.y - 1][CalculateIdx(gridPos.x, gridPos.z)] == eGridType::PLAYER)
		return true;

	return false;
}

bool GridManager::IsPlayerAbove(Vector3 pos) const
{
	GridPos gridPos = ConvertToGridPos(pos);
	if (gridPos.y == m_numHeight - 1)
		return false;

	if (m_gridMap[gridPos.y + 1][CalculateIdx(gridPos.x, gridPos.z)] == eGridType::PLAYER)
		return true;

	return false;
}

std::vector<std::pair<MinimapPos, eMinimapType>>& GridManager::GetMinimapInfo()
{
	for(int x=0; x<m_miniMap.size(); ++x)
	{
		for(int z=0; z<m_miniMap[x].size(); ++z)
		{
			if(m_miniMap[x][z] != eMinimapType::EMPTY)
			{
				m_miniMapInfos.emplace_back(std::make_pair(MinimapPos{ x, z }, m_miniMap[x][z]));
			}
		}
	}

	return m_miniMapInfos;
}

void GridManager::ClearMinimapInfo()
{
	m_miniMapInfos.clear();
}

GridPos& GridManager::ConvertToGridPos(Vector3 position) const
{
	GridPos rt;
	rt.x = std::round(position.x - m_startXPos) / m_gridSize;
	rt.y = std::round(position.y - m_startYPos) / m_gridSize;
	rt.z = std::round(position.z - m_startZPos) / m_gridSize;

	return rt;
}

Vector3& GridManager::ConvertToDXPos(GridPos position) const
{
	Vector3 rt;
	rt.x = position.x * m_gridSize + m_startXPos + m_gridSize / 2;
	rt.y = position.y * m_gridSize + m_startYPos + m_gridSize / 2;
	rt.z = position.z * m_gridSize + m_startZPos + m_gridSize / 2;

	return rt;
}

bool GridManager::CheckDuplicated(GridPos& gridPos) const
{
	if (m_gridMap[gridPos.y][CalculateIdx(gridPos.x, gridPos.z)] == eGridType::EMPTY)
		return false;
	return true;
}

eGridType GridManager::MatchingGridType(eObjectType type) const
{
	switch (type)
	{
	case eObjectType::LEVEL:
		return eGridType::BLOCK;
	case eObjectType::PLAYER:
		return eGridType::PLAYER;
	case eObjectType::PART:
		return eGridType::PART;
	default:
		LOG_CONTENTS("정의되지 않은 오브젝트, 그리드 매칭");
		assert(true);
		return eGridType::EMPTY;
	}
}

bool GridManager::CheckMovePossibility(GridPos& gridPos, Vector3 vec)
{
	bool rt = false;
	GridPos checkPos = gridPos + vec;

	if (checkPos.x < 0 || checkPos.x >= m_numCol
		|| checkPos.y < 0 || checkPos.y >= m_numHeight
		|| checkPos.z < 0 || checkPos.z >= m_numRow)	// z_up
		return rt;

	if (m_gridMap[checkPos.y][CalculateIdx(checkPos.x, checkPos.z)] == eGridType::EMPTY
	|| m_gridMap[checkPos.y][CalculateIdx(checkPos.x, checkPos.z)] == eGridType::PART)
		rt = true;

	return rt;
}