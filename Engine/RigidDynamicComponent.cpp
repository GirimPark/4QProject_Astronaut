#include "pch.h"
#include "RigidDynamicComponent.h"
#include "PhysicsManager.h"
#include "GameObject.h"

RigidDynamicComponent::~RigidDynamicComponent()
{
	PhysicsManager::GetInstance()->RemoveCollisionHandler(m_rigidDynamic);
	PhysicsManager::GetInstance()->RemoveCollisionOwner(m_rigidDynamic);
	m_rigidDynamic->release();
	PhysicsManager::GetInstance()->RemoveRigidDynamicComponent(this);
}

PxRigidDynamic* RigidDynamicComponent::CreateDynamicRigidBody(Geometry geometryType, const std::vector<float>& geometryArgs, const std::vector<float>& materialArgs, float density, const Math::Vector3& offsetTrasnform, const Math::Vector3& offsetRotation, bool isTrigger)
{
	std::shared_ptr<PxGeometry> geometry;
	switch (geometryType)
	{
	case RigidBodyComponent::Geometry::Sphere:
	{
		geometry = CreateGeometry<Geometry::Sphere>(geometryArgs[0]);
	}
	break;
	case RigidBodyComponent::Geometry::Capsule:
	{
		geometry = CreateGeometry<Geometry::Capsule>(geometryArgs[0], geometryArgs[1]);
	}
	break;
	case RigidBodyComponent::Geometry::Box:
	{
		geometry = CreateGeometry<Geometry::Box>(geometryArgs[0], geometryArgs[1], geometryArgs[2]);
	}
	break;
	default:
	{
		LOG_ENGINE("CreateDynamicRigidBody Argument Error");
		assert(false);
		return nullptr;
	}
	break;
	}
	PxMaterial* material = CreateMaterial(materialArgs[0], materialArgs[1], materialArgs[2]);
	m_rigidDynamic = PxCreateDynamic(*PhysicsManager::GetInstance()->GetPhysics(), ConvertPxTransform(), *geometry, *material, density);

	if (m_rigidDynamic == nullptr)
	{
		LOG_ENGINE("CreateDynamicRigidBody Error");
		assert(false);
		return nullptr;

	}
	SetCollisionFilter(m_rigidDynamic);

	PxShape* sensorShape;
	m_rigidDynamic->getShapes(&sensorShape, 1);
	if (isTrigger)
	{
		PxShape* sensorShape;
		m_rigidDynamic->getShapes(&sensorShape, 1);
		sensorShape->setFlag(PxShapeFlag::eSIMULATION_SHAPE, false);
		sensorShape->setFlag(PxShapeFlag::eTRIGGER_SHAPE, true);
	}

	if (offsetTrasnform != Math::Vector3(0.f, 0.f, 0.f)
		|| offsetRotation != Math::Vector3(0.f, 0.f, 0.f))
	{
		auto quat = Math::Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(offsetRotation.y), XMConvertToRadians(offsetRotation.x), XMConvertToRadians(offsetRotation.z));

		sensorShape->setLocalPose(PxTransform(physx::PxVec3T(offsetTrasnform.x, offsetTrasnform.y, offsetTrasnform.z), physx::PxQuat(quat.x, quat.y, quat.z, quat.w)));
	}

	if (geometryType == Geometry::Capsule)
	{
		PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
		sensorShape->setLocalPose(relativePose);
		PxRigidBodyExt::updateMassAndInertia(*m_rigidDynamic, density);
	}

#ifdef _DEBUG
	m_rigidDynamic->setActorFlag(PxActorFlag::eVISUALIZATION, true);
#endif
	material->release();

	PhysicsManager::GetInstance()->AddCollisionOwner(m_rigidDynamic, GetOwner());
	return m_rigidDynamic;
}

void RigidDynamicComponent::AddTriggerShape(Geometry geometryType, const std::vector<float>& geometryArgs, const Math::Vector3& offsetTrasnform, const Math::Vector3& offsetRotation)
{
	if (m_rigidDynamic == nullptr)
	{
		LOG_ENGINE("RigidDynamicComponent not initialized");
		assert(false);
		return;
	}

	std::shared_ptr<PxGeometry> geometry;
	switch (geometryType)
	{
	case RigidBodyComponent::Geometry::Sphere:
	{
		geometry = CreateGeometry<Geometry::Sphere>(geometryArgs[0]);
	}
	break;
	case RigidBodyComponent::Geometry::Capsule:
	{
		geometry = CreateGeometry<Geometry::Capsule>(geometryArgs[0], geometryArgs[1]);
	}
	break;
	case RigidBodyComponent::Geometry::Box:
	{
		geometry = CreateGeometry<Geometry::Box>(geometryArgs[0], geometryArgs[1], geometryArgs[2]);
	}
	break;
	default:
	{
		LOG_ENGINE("CreateDynamicRigidBody Argument Error");
		assert(false);
		return;
	}
	break;
	}

	PxMaterial* triggerMaterial = CreateMaterial(0.0f, 0.0f, 0.0f);
	const PxShapeFlags shapeFlags = PxShapeFlag::eVISUALIZATION | PxShapeFlag::eTRIGGER_SHAPE;
	PxShape* triggerShape = PhysicsManager::GetInstance()->GetPhysics()->createShape(*geometry, *triggerMaterial, false, shapeFlags);

	if (triggerShape)
	{

		if (offsetTrasnform != Math::Vector3(0.f, 0.f, 0.f)
			|| offsetRotation != Math::Vector3(0.f, 0.f, 0.f))
		{
			auto quat = Math::Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(offsetRotation.y), XMConvertToRadians(offsetRotation.x), XMConvertToRadians(offsetRotation.z));

			triggerShape->setLocalPose(PxTransform(physx::PxVec3T(offsetTrasnform.x, offsetTrasnform.y, offsetTrasnform.z), physx::PxQuat(quat.x, quat.y, quat.z, quat.w)));
		}

		m_rigidDynamic->attachShape(*triggerShape);
	}

	triggerMaterial->release();
	triggerShape->release();
}

void RigidDynamicComponent::SetMass(float mass, Math::Vector3 centerOfMassLocalPos)
{
	assert(m_rigidDynamic != nullptr);

	PxVec3 pos = { centerOfMassLocalPos.x, centerOfMassLocalPos.y, centerOfMassLocalPos.z };

	PxRigidBodyExt::setMassAndUpdateInertia(*m_rigidDynamic, mass, &pos);
}

void RigidDynamicComponent::SetLockFlags(PxRigidDynamicLockFlags flags)
{
	assert(m_rigidDynamic != nullptr);
	m_rigidDynamic->setRigidDynamicLockFlags(flags);
}

void RigidDynamicComponent::SetTransform(Math::Matrix& worldMatrix)
{
	PxTransform newTransform;

	Math::Vector3 position;
	Math::Vector3 scale;
	Quaternion rotationQuat;
	worldMatrix.Decompose(scale, rotationQuat, position);

	newTransform.p = PxVec3(position.x, position.y, position.z);
	newTransform.q = PxQuat(rotationQuat.x, rotationQuat.y, rotationQuat.z, rotationQuat.w);

	m_rigidDynamic->setGlobalPose(newTransform);
}

void RigidDynamicComponent::Initialize()
{
	PhysicsManager::GetInstance()->AddRigidDynamicComponent(this);
	//m_rigidDynamic->setAngularDamping(0.5f);
	PhysicsManager::GetInstance()->GetScene()->addActor(*m_rigidDynamic);
}

void RigidDynamicComponent::Update(float deltaTime)
{
	PxTransform tf = m_rigidDynamic->getGlobalPose();

	auto euler = Math::Quaternion(tf.q.x, tf.q.y, tf.q.z, tf.q.w).ToEuler();

	m_pOwner.lock()->SetPosition({ tf.p.x, tf.p.y, tf.p.z });
	m_pOwner.lock()->SetRotation({ XMConvertToDegrees(euler.x), XMConvertToDegrees(euler.y), XMConvertToDegrees(euler.z) });
}