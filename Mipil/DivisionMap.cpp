#include "pch.h"
#include "DivisionMap.h"
#include "StaticFbxObject.h"
#include "PuzzleWorld.h"
#include "PlayerCamera.h"

#include "../Engine/StaticMeshComponent.h"

DivisionMap::DivisionMap(PuzzleWorld* world)
{
	m_owner = world;
}

DivisionMap::~DivisionMap()
{

}

void DivisionMap::Update()
{
	UpdateVisibility();
}

void DivisionMap::SetMaps()
{
	m_front = m_owner->CreateGameObject<StaticFbxObject>("m_front", eObjectType::TEST).lock();
	m_front->SetDeferredPosition(m_deferredPosition);
	m_front->SetFilePath("../Resources/FBX/front.fbx");

	m_back = m_owner->CreateGameObject<StaticFbxObject>("m_back", eObjectType::TEST).lock();
	m_back->SetDeferredPosition(m_deferredPosition);
	m_back->SetFilePath("../Resources/FBX/back.fbx");

	m_left = m_owner->CreateGameObject<StaticFbxObject>("m_left", eObjectType::TEST).lock();
	m_left->SetDeferredPosition(m_deferredPosition);
	m_left->SetFilePath("../Resources/FBX/left.fbx");

	m_right = m_owner->CreateGameObject<StaticFbxObject>("m_right", eObjectType::TEST).lock();
	m_right->SetDeferredPosition(m_deferredPosition);
	m_right->SetFilePath("../Resources/FBX/right.fbx");

	m_deco = m_owner->CreateGameObject<StaticFbxObject>("m_deco", eObjectType::TEST).lock();
	m_deco->SetDeferredPosition(m_deferredPosition);
	m_deco->SetFilePath("../Resources/FBX/ingame_deco.fbx");
}

void DivisionMap::SetCamera(PlayerCamera* camera)
{
	if (m_owner != nullptr)
	{
		assert(camera);
		m_camera = camera;
	}
}

void DivisionMap::SetVisibleFront(bool isVisible)
{
	m_front->GetWeakMeshComponent().lock()->SetVisible(isVisible);
}

void DivisionMap::SetVisibleBack(bool isVisible)
{
	m_back->GetWeakMeshComponent().lock()->SetVisible(isVisible);
}

void DivisionMap::SetVisibleLeft(bool isVisible)
{
	m_left->GetWeakMeshComponent().lock()->SetVisible(isVisible);
}

void DivisionMap::SetVisibleRight(bool isVisible)
{
	m_right->GetWeakMeshComponent().lock()->SetVisible(isVisible);
}

void DivisionMap::UpdateVisibility()
{
	auto& subjectPos = m_camera->GetSubject()->GetWorldPosition();
	auto& cameraPos = m_camera->GetWorldPosition();
	Math::Vector3 cameraToSubject = subjectPos - cameraPos;
	cameraToSubject.Normalize();

	float angle = XMConvertToDegrees(std::atan2(cameraToSubject.z, cameraToSubject.x));

	angle = fmod(angle + 360.0f, 360.0f);

	SetVisibleFront(!(angle >= 215.0f && angle < 325.0f));
	SetVisibleRight(!(angle >= 125.0f && angle < 235.0f));
	SetVisibleLeft(!(angle >= 35.0f && angle < 145.0f));
	SetVisibleBack(!(angle >= 305.0f || angle < 55.0f));
}
