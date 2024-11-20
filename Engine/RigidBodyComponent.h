#pragma once
#include "PxPhysicsAPI.h"
#include "PhysicsManager.h"
#include "Component.h"

using namespace physx;
class CollisionHandler;

/// <summary>
/// RigidBodyComponent
/// RigidDynamicComponent, RigidStaticComponent 클래스를 위한 베이스 클래스
/// </summary>
class RigidBodyComponent :
	public Component
{
public:
	enum class Geometry {
		Sphere,
		Capsule,
		Box,
		END
	};

	virtual ~RigidBodyComponent();
	// Component을(를) 통해 상속됨
	void Initialize() override;
	void Update(float deltaTime) override;

protected:
	PxTransform ConvertPxTransform();

	template <Geometry Sphere>
	std::shared_ptr<PxGeometry> CreateGeometry(float radius);
	template <Geometry Capsule>
	std::shared_ptr<PxGeometry> CreateGeometry(float radius, float halfHeight);
	template <Geometry Box>
	std::shared_ptr<PxGeometry> CreateGeometry(float halfX, float halfY, float halfZ);

	PxMaterial* CreateMaterial(float staticFriction, float dynamicFriction, float restitution);
	void SetCollisionFilter(PxRigidActor* actor);

	std::shared_ptr<CollisionHandler> m_collisionHandler;
};


template<>
inline std::shared_ptr<PxGeometry> RigidBodyComponent::CreateGeometry<RigidBodyComponent::Geometry::Sphere>(float radius)
{
	return std::make_shared<PxSphereGeometry>(radius);
}

template<>
inline std::shared_ptr<PxGeometry> RigidBodyComponent::CreateGeometry<RigidBodyComponent::Geometry::Capsule>(float radius, float halfHeight)
{
	return std::make_shared<PxCapsuleGeometry>(radius, halfHeight);
}

template<>
inline std::shared_ptr<PxGeometry> RigidBodyComponent::CreateGeometry<RigidBodyComponent::Geometry::Box>(float halfX, float halfY, float halfZ)
{
	return std::make_shared<PxBoxGeometry>(halfX, halfY, halfZ);
}
