#pragma once

#include <directxtk/SimpleMath.h>
namespace Math = DirectX::SimpleMath;

class GameObject;
/// <summary>
/// 부모의 위치를 반영한 위치 업데이트를 위한 컴포넌트
///	믹스인으로 상속받아 사용한다
/// </summary>
class Transform
{
public:
	Transform();
	~Transform() = default;

	const Math::Vector3 GetWorldPosition() const;

	void SetParent(Transform* pParent) { m_pParent = pParent; }

	virtual void Update(float deltaTime);

	Math::Matrix m_localMatrix;
	Math::Matrix m_worldMatrix;

	GameObject* m_owner;
	Transform* m_pParent = nullptr;
};

