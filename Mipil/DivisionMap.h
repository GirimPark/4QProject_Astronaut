#pragma once
#include "../Engine/GameObject.h"
#include "../Engine/World.h"

class StaticMeshComponent;
class StaticFbxObject;
class PuzzleWorld;
class PlayerCamera;

class DivisionMap
{
public:
	DivisionMap(PuzzleWorld* world);
	~DivisionMap();

public:
	void Update();

	void SetMaps();
	void SetCamera(PlayerCamera* camera);
	void SetVisibleFront(bool isVisible);
	void SetVisibleBack(bool isVisible);
	void SetVisibleLeft(bool isVisible);
	void SetVisibleRight(bool isVisible);

	void SetDeferredPosition(Vector3 pos) { m_deferredPosition = pos; }

	std::shared_ptr<StaticFbxObject> GetFront() { return m_front; }
	std::shared_ptr<StaticFbxObject> GetBack() { return m_back; }
	std::shared_ptr<StaticFbxObject> GetLeft() { return m_left; }
	std::shared_ptr<StaticFbxObject> GetRight() { return m_right; }
	std::shared_ptr<StaticFbxObject> GetDeco() { return m_deco; }

private:
	void UpdateVisibility();

private:
	std::shared_ptr<StaticFbxObject> m_front;
	std::shared_ptr<StaticFbxObject> m_back;
	std::shared_ptr<StaticFbxObject> m_left;
	std::shared_ptr<StaticFbxObject> m_right;
	std::shared_ptr<StaticFbxObject> m_deco;

	PuzzleWorld* m_owner;
	PlayerCamera* m_camera;

	Vector3 m_deferredPosition;
};

