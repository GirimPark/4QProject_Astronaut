#pragma once
#include "SceneComponent.h"

/// <summary>
/// 렌더링이 필요한 컴포넌트는 해당 클래스를 상속받아 구현한다.
///	AABB를 가져 카메라 컬링하는 것이 주된 목표
/// 인스턴스화하지 않는다.
/// </summary>
class RenderComponent
	: public SceneComponent
{
public:
	RenderComponent() = default;
	~RenderComponent() override = default;

public:
	Math::Vector3 GetBoundingBoxOffset() const { return m_boundingBoxOffset; }
	void SetBoundingBoxOffset(Math::Vector3 offset) { m_boundingBoxOffset = offset; }
	void SetVisible(bool visible) { m_bVisible = visible; }

public:
	void Update(float deltaTime) override;


protected:
	int m_renderOrder = 0;
	bool m_bCulled = true;
	bool m_bVisible = true;

	DirectX::BoundingBox m_boundingBox;
	Math::Vector3 m_boundingBoxOffset = Math::Vector3::Zero;
};

