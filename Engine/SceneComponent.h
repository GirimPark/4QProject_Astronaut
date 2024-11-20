#pragma once

#include "Component.h"
#include "Transform.h"

/// <summary>
/// 위치정보를 갖는 Component
/// 일반적으로는 인스턴스화하지 않는다.
/// 위치정보를 필요로 하는 컴포넌트들은 해당 컴포넌트를 상속받아 사용한다.
/// </summary>
class SceneComponent
	: public Component
	, public Transform
{
public:
	SceneComponent() = default;
	~SceneComponent() override = default;

public:
	void SetLocalPosition(const Math::Vector3 pos);
	void SetLocalScale(const Math::Vector3 scale);
	void SetLocalRotation(const Math::Vector3 rot);
	void SetLocalTransform(Math::Matrix matrix);

	Math::Vector3& GetLocalPosition() { return m_localPosition; }
	Math::Vector3& GetLocalScale() { return m_localScale; }
	Math::Vector3& GetLocalRotation() { return m_localRotation; }
	Math::Matrix& GetLocalTransform() { return m_localMatrix; }
	Vector3& GetWorldPosition() { return m_worldPosition; }
	Vector3& GetWorldScale() { return m_worldScale; }
	Vector3& GetWorldRotation() { return m_worldRotation; }
	Math::Matrix& GetWorldTransform() { return m_worldMatrix; }
	Math::Vector3& GetForward() const;

public:
	void Initialize() override;
	void Update(float deltaTime) override;

private:
	Math::Vector3 m_localPosition = Math::Vector3(0.f, 0.f, 0.f);
	Math::Vector3 m_localScale = Math::Vector3(1.f, 1.f, 1.f);
	Math::Vector3 m_localRotation = Math::Vector3(0.f, 0.f, 0.f);
	Vector3 m_worldPosition;
	Vector3 m_worldScale;
	Vector3 m_worldRotation;

	bool m_bCalced = false;	// 매 프레임 매트릭스를 만들어내지 않기 위한 변수
	bool m_bCalcedWorld = false;	// 매 프레임 매트릭스를 만들어내지 않기 위한 변수
};

