#pragma once
#include "PxPhysicsAPI.h"
#include "Singleton.h"
#include "CollisionFilter.h"
#include "CollisionEventCallback.h"
/// <summary>
/// PhysicsManager
/// </summary>

class RigidDynamicComponent;
class RigidStaticComponent;
class CollisionHandler;

using namespace physx;

class PhysicsManager
	: public Singleton<PhysicsManager>
{

public:
	~PhysicsManager();

	void AddRigidDynamicComponent(RigidDynamicComponent* rigidComponents);
	void AddRigidStaticComponent(RigidStaticComponent* rigidComponents);
	void RemoveRigidDynamicComponent(RigidDynamicComponent* rigidComponents);
	void RemoveRigidStaticComponent(RigidStaticComponent* rigidComponents);

	void AddCollisionHandler(physx::PxActor* actor, std::weak_ptr<CollisionHandler> collisionHandler) { m_collisionHandlers[actor] = collisionHandler; }
	void AddCollisionOwner(physx::PxActor* actor, std::weak_ptr<GameObject> collisionOwner) { m_collisionOwner[actor] = collisionOwner; }
	void RemoveCollisionHandler(physx::PxActor* actor);
	void RemoveCollisionOwner(physx::PxActor* actor);

	physx::PxPhysics* GetPhysics() { return m_physics; }
	physx::PxScene* GetScene() { return m_scene; }
	physx::PxFoundation* GetFoundation() { return m_foundation; }
	physx::PxDefaultErrorCallback& GetErrorCallback() { return m_errorCallback; }
	std::weak_ptr<CollisionHandler> GetCollisionHandler(physx::PxActor* actor);
	std::weak_ptr<GameObject> GetCollisionOwner(physx::PxActor* actor);

	void Initailize();
	void Finalize();
	void Update(float deltaTime);
	void StepPhysics(float deltaTime);

private:
	physx::PxDefaultAllocator m_allocator;
	physx::PxDefaultErrorCallback m_errorCallback;
	physx::PxFoundation* m_foundation = nullptr;
	physx::PxPhysics* m_physics = nullptr;
	physx::PxDefaultCpuDispatcher* m_dispatcher = nullptr;
	physx::PxScene* m_scene = nullptr;
	physx::PxPvd* m_pvd = nullptr;
	static std::unique_ptr<CollisionFilter> m_CollisionFilter;
	std::unique_ptr<CollisionEventCallback> m_CollisionEventCallback;

	float m_accumulator = 0.0f;
	float m_stepSize = 1.0f / 60.0f;

	std::list<RigidDynamicComponent*> m_rigidDynamicComponents;
	std::list<RigidStaticComponent*> m_rigidStaticComponents;
	std::unordered_map<physx::PxActor*, std::weak_ptr<CollisionHandler>> m_collisionHandlers;
	std::unordered_map<physx::PxActor*, std::weak_ptr<GameObject>> m_collisionOwner;
};