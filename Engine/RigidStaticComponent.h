#pragma once
#include "PxPhysicsAPI.h"
#include "RigidBodyComponent.h"

using namespace physx;

/// <summary>
/// RigidStaticComponent
/// 멈춰 있는 정적 개체를 위한 스태틱 리지드 바디 컴포넌트
/// CreateStaticRigidBody로 리지드 바디 생성함. 인자 설정 방법은 주석 참고
/// CreateStaticRigidBody 할 때 isTrigger true 하면 리지드바디 자체가 트리거로 생성되고,
/// AddTriggerShape 함수로 추가 트리거 영역도 설정 해줄 수 있다.
/// 평면 생성할 땐 CreatePlane 함수 사용
/// </summary>

class StaticMeshComponent;

class RigidStaticComponent
	: public RigidBodyComponent, public std::enable_shared_from_this<RigidStaticComponent>
{
public:
	virtual ~RigidStaticComponent();

	PxRigidStatic* GetRigidStatic() { return m_rigidStatic; }

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
	PxRigidStatic* CreateStaticRigidBody(Geometry geometryType, const std::vector<float>& geometryArgs, const std::vector<float>& materialArgs, const Math::Vector3& offsetTrasnform = { 0.f, 0.f, 0.f }, const Math::Vector3& offsetRotation = { 0.f, 0.f, 0.f }, bool isTrigger = false);

	PxRigidStatic* CreateStaticRigidBodyFromStaticMesh(const StaticMeshComponent* staticMeshComponent, const std::vector<float>& materialArgs);

	void AddTriggerShape(Geometry geometryType, const std::vector<float>& geometryArgs, const Math::Vector3& offsetTrasnform = { 0.f, 0.f, 0.f }, const Math::Vector3& offsetRotation = { 0.f, 0.f, 0.f });

	PxRigidStatic* CreatePlane(float nx, float ny, float nz, float distance, const std::vector<float>& materialArgs);

	PxRigidStatic* CreateHollowBox(float Width, float Height, float Depth, const std::vector<float>& materialArgs);

	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;

private:
	PxRigidStatic* m_rigidStatic;
};

template<typename CollisionHandlerType>
	requires std::is_base_of<CollisionHandler, CollisionHandlerType>::value
inline void RigidStaticComponent::AddCollisionHandler()
{
	if (!m_collisionHandler) {
		m_collisionHandler = PhysicsManager::GetInstance()->GetCollisionHandler(m_rigidStatic).lock();
		if (!m_collisionHandler) {
			m_collisionHandler = std::make_shared<CollisionHandlerType>();
			PhysicsManager::GetInstance()->AddCollisionHandler(m_rigidStatic, m_collisionHandler);
		}
	}
}