#pragma once

#include "Object.h"

class GameObject;

/// <summary>
/// 가장 상위의 Component
/// Component의 공통 정보인 부모 오브젝트, 이름을 가진다.
/// 대략적인 Component 구조는 아래와 같다.
/// Component
///   -SceneComponent
///     -CameraComponent
///     -RenderComponent
///       -RenderableComponents...
///   -MovementComponent
///   -FSMComponent
/// </summary>
class Component
	: public Object
{
public:
	Component() = default;
	virtual ~Component() = default;

public:
	void SetOwner(std::shared_ptr<GameObject> pOwner) { m_pOwner = pOwner; }
	void SetName(std::string_view name) { m_name = name; }

	std::weak_ptr<GameObject> GetOwner() const { return m_pOwner; }

public:
	virtual void Initialize() abstract;
	virtual void Update(float deltaTime) abstract;


protected:
	std::weak_ptr<GameObject> m_pOwner;
	std::string m_name;
};

