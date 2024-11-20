#pragma once
#include "PxPhysicsAPI.h"
#include "RigidBodyComponent.h"

using namespace physx;

/// <summary>
/// RigidDynamicComponent
/// 움직이는 동적 개체를 위한 다이나믹 리지드 바디 컴포넌트
/// CreateDynamicRigidBody로 리지드 바디 생성함. 인자 설정 방법은 주석 참고
/// CreateDynamicRigidBody 할 때 isTrigger true 하면 리지드바디 자체가 트리거로 생성되고,
/// AddTriggerShape 함수로 추가 트리거 영역도 설정 해줄 수 있다.
/// CreateDynamicRigidBody 인자 중에 density 밀도로 무게 설정하는건데
/// SetMass함수로 강제로 설정할 수도 있다. (테스트 안 해봄)
/// SetLockFlags로 이동이나 회전에 락 걸 수 있음. 인자는 PxRigidDynamicLockFlags 참고
/// </summary>
class RigidDynamicComponent
	: public RigidBodyComponent, public std::enable_shared_from_this<RigidDynamicComponent>
{
public:
	virtual ~RigidDynamicComponent();
	PxRigidDynamic* GetRigidDynamic() { return m_rigidDynamic; }

	template<typename CollisionHandlerType>
		requires std::is_base_of<CollisionHandler, CollisionHandlerType>::value
	void AddCollisionHandler();
	//인자 타입별 갯수 맞춰서 {}로 전달
	//geometryArgs
	//Sphere : radian
	//Capsule : radian, halfHeight
	//Box : halfX, halfY, halfZ
	//materialArgs
	//staticFriction : 정지 마찰력
	//dynamicFriction : 운동 마찰력
	//restitution : 반발 계수
	//density : 밀도
	PxRigidDynamic* CreateDynamicRigidBody(Geometry gesometryType, const std::vector<float>& geometryArgs, const std::vector<float>& materialArgs, float density = 1.f, const Math::Vector3& offsetTrasnform = { 0.f, 0.f, 0.f }, const Math::Vector3& offsetRotation = { 0.f, 0.f, 0.f }, bool isTrigger = false);

	void AddTriggerShape(Geometry geometryType, const std::vector<float>& geometryArgs, const Math::Vector3& offsetTrasnform = { 0.f, 0.f, 0.f }, const Math::Vector3& offsetRotation = { 0.f, 0.f, 0.f });

	void SetMass(float mass, Math::Vector3 centerOfMassLocalPos = { 0.f, 0.f, 0.f });
	void SetLockFlags(PxRigidDynamicLockFlags flags);
	void SetTransform(Math::Matrix& worldMatrix);

	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;

private:
	PxRigidDynamic* m_rigidDynamic;
};

template<typename CollisionHandlerType>
	requires std::is_base_of<CollisionHandler, CollisionHandlerType>::value
inline void RigidDynamicComponent::AddCollisionHandler()
{
	if (!m_collisionHandler) {
		m_collisionHandler = PhysicsManager::GetInstance()->GetCollisionHandler(m_rigidDynamic).lock();
		if (!m_collisionHandler) {
			m_collisionHandler = std::make_shared<CollisionHandlerType>();
			PhysicsManager::GetInstance()->AddCollisionHandler(m_rigidDynamic, m_collisionHandler);
		}
	}
}