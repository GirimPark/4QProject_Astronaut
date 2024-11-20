#include "pch.h"
#include "RigidStaticComponent.h"
#include "PhysicsManager.h"
#include "StaticMeshComponent.h"
#include "../D3DRenderer/StaticMeshModel.h"
#include "../D3DRenderer/Mesh.h"

#include "cooking/PxCooking.h"
#include "cooking/PxConvexMeshDesc.h"
#include "cooking/PxTriangleMeshDesc.h"
#include "foundation/PxIO.h"
#include "foundation/PxPreprocessor.h"
//#include "cooking/PxConvexMeshGeometry.h"
//#include "cooking/PxConvexMeshCookingResult.h"
//#include "cooking/PxCookingParams.h"
//#include "cooking/PxTriangleMesh.h"
//#include "cooking/PxTriangleMeshCookingResult.h"

RigidStaticComponent::~RigidStaticComponent()
{
	PhysicsManager::GetInstance()->RemoveCollisionHandler(m_rigidStatic);
	PhysicsManager::GetInstance()->RemoveCollisionOwner(m_rigidStatic);
	m_rigidStatic->release();
	PhysicsManager::GetInstance()->RemoveRigidStaticComponent(this);
}

PxRigidStatic* RigidStaticComponent::CreateStaticRigidBody(Geometry geometryType, const std::vector<float>& geometryArgs, const std::vector<float>& materialArgs, const Math::Vector3& offsetTrasnform, const Math::Vector3& offsetRotation, bool isTrigger)
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
		LOG_ENGINE("CreateStaticRigidBody Argument Error");
		assert(false);
		return nullptr;
	}
	break;
	}

	PxMaterial* material = CreateMaterial(materialArgs[0], materialArgs[1], materialArgs[2]);

	m_rigidStatic = PxCreateStatic(*PhysicsManager::GetInstance()->GetPhysics(), ConvertPxTransform(), *geometry, *material);

	if (m_rigidStatic == nullptr)
	{
		LOG_ENGINE("CreateStaticRigidBody Error");
		assert(false);
		return nullptr;
	}

	SetCollisionFilter(m_rigidStatic);

	PxShape* sensorShape;
	m_rigidStatic->getShapes(&sensorShape, 1);
	if (isTrigger)
	{
		PxShape* sensorShape;
		m_rigidStatic->getShapes(&sensorShape, 1);
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
	}

#ifdef _DEBUG
	m_rigidStatic->setActorFlag(PxActorFlag::eVISUALIZATION, true);
#endif

	material->release();

	PhysicsManager::GetInstance()->AddCollisionOwner(m_rigidStatic, GetOwner());
	return m_rigidStatic;
}

PxRigidStatic* RigidStaticComponent::CreateStaticRigidBodyFromStaticMesh(const StaticMeshComponent* staticMeshComponent, const std::vector<float>& materialArgs)
{
	const auto& staticMeshModel = staticMeshComponent->GetStaticMeshModel();

	if (staticMeshModel == nullptr)
	{
		LOG_ENGINE("StaticMeshComponent does not have a valid StaticMeshModel");
		assert(false);
		return nullptr;
	}

	const std::vector<Mesh>& meshes = staticMeshModel->GetMeshes();

	if (meshes.empty())
	{
		LOG_ENGINE("StaticMeshModel does not have any meshes");
		assert(false);
		return nullptr;
	}
	std::vector<Vertex> combinedVertices;
	std::vector<UINT> combinedIndices;
	size_t currentIndexOffset = 0;

	for (const Mesh& mesh : meshes)
	{
		combinedVertices.insert(combinedVertices.end(), mesh.m_Vertices.begin(), mesh.m_Vertices.end());

		for (UINT index : mesh.m_Indices)
		{
			combinedIndices.push_back(index + static_cast<UINT>(currentIndexOffset));
		}

		currentIndexOffset += mesh.m_VertexCount;
	}

	auto* Phys = PhysicsManager::GetInstance();

	PxTolerancesScale scale;
	PxCookingParams params(scale);
	// disable mesh cleaning - perform mesh validation on development configurations
	params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH;
	// disable edge precompute, edges are set for each triangle, slows contact generation
	params.meshPreprocessParams |= PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE;

	PxTriangleMeshDesc meshDesc;
	meshDesc.points.count = static_cast<PxU32>(combinedVertices.size());
	meshDesc.points.stride = sizeof(Vertex);
	meshDesc.points.data = reinterpret_cast<const PxVec3*>(combinedVertices.data());

	meshDesc.triangles.count = static_cast<PxU32>(combinedIndices.size() / 3);
	meshDesc.triangles.stride = 3 * sizeof(UINT);
	meshDesc.triangles.data = reinterpret_cast<const void*>(combinedIndices.data());

	PxDefaultMemoryOutputStream writeBuffer;
	PxTriangleMeshCookingResult::Enum result;
	bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);

	if (!status)
	{
		LOG_ENGINE("CookTriangleMesh Error");
		assert(false);
		return nullptr;
	}

	PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
	PxTriangleMesh* triangleMesh = Phys->GetPhysics()->createTriangleMesh(readBuffer);

	PxMaterial* material = CreateMaterial(materialArgs[0], materialArgs[1], materialArgs[2]);

	m_rigidStatic = Phys->GetPhysics()->createRigidStatic(ConvertPxTransform());

	if (m_rigidStatic == nullptr)
	{
		LOG_ENGINE("CreateStaticRigidBody Error");
		assert(false);
		return nullptr;
	}

	PxShape* shape = Phys->GetPhysics()->createShape(PxTriangleMeshGeometry(triangleMesh), *material);
	m_rigidStatic->attachShape(*shape);

	SetCollisionFilter(m_rigidStatic);

	triangleMesh->release();
	material->release();
	shape->release();

	PhysicsManager::GetInstance()->AddCollisionOwner(m_rigidStatic, GetOwner());

	return m_rigidStatic;
}

void RigidStaticComponent::AddTriggerShape(Geometry geometryType, const std::vector<float>& geometryArgs, const Math::Vector3& offsetTrasnform, const Math::Vector3& offsetRotation)
{
	if (m_rigidStatic == nullptr)
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
		if (geometryType == Geometry::Capsule)
		{
			PxTransform relativePose(PxQuat(PxHalfPi, PxVec3(0, 0, 1)));
			triggerShape->setLocalPose(relativePose);
		}

		if (offsetTrasnform != Math::Vector3(0.f, 0.f, 0.f)
			|| offsetRotation != Math::Vector3(0.f, 0.f, 0.f))
		{
			auto quat = Math::Quaternion::CreateFromYawPitchRoll(XMConvertToRadians(offsetRotation.y), XMConvertToRadians(offsetRotation.x), XMConvertToRadians(offsetRotation.z));

			triggerShape->setLocalPose(PxTransform(physx::PxVec3T(offsetTrasnform.x, offsetTrasnform.y, offsetTrasnform.z), physx::PxQuat(quat.x, quat.y, quat.z, quat.w)));
		}

		m_rigidStatic->attachShape(*triggerShape);
	}

	triggerMaterial->release();
	triggerShape->release();
}

PxRigidStatic* RigidStaticComponent::CreatePlane(float nx, float ny, float nz, float distance, const std::vector<float>& materialArgs)
{
	PxMaterial* material = CreateMaterial(materialArgs[0], materialArgs[1], materialArgs[2]);

	m_rigidStatic = PxCreatePlane(*PhysicsManager::GetInstance()->GetPhysics(), PxPlane(nx, ny, nz, distance).transform(ConvertPxTransform()), *material);

	if (m_rigidStatic == nullptr)
	{
		LOG_ENGINE("CreatePlane Error");
		assert(false);
		return nullptr;
	}

	SetCollisionFilter(m_rigidStatic);

#ifdef _DEBUG
	m_rigidStatic->setActorFlag(PxActorFlag::eVISUALIZATION, true);
#endif

	material->release();

	PhysicsManager::GetInstance()->AddCollisionOwner(m_rigidStatic, GetOwner());
	return m_rigidStatic;
}

PxRigidStatic* RigidStaticComponent::CreateHollowBox(float Width, float Height, float Depth, const std::vector<float>& materialArgs)
{

	float halfWidth = Width * 0.5f;
	float halfHeight = Height * 0.5f;
	float halfDepth = Depth * 0.5f;

	PxMaterial* material = CreateMaterial(materialArgs[0], materialArgs[1], materialArgs[2]);

	PxTransform transforms[6] = {
		PxTransform(PxVec3(halfWidth, 0.0f, 0.0f)),    // Right
		PxTransform(PxVec3(-halfWidth, 0.0f, 0.0f)),   // Left
		PxTransform(PxVec3(0.0f, halfHeight, 0.0f)),    // Top
		PxTransform(PxVec3(0.0f, -halfHeight, 0.0f)),   // Bottom
		PxTransform(PxVec3(0.0f, 0.0f, halfDepth)),    // Front
		PxTransform(PxVec3(0.0f, 0.0f, -halfDepth))    // Back
	};

	float thickness = 0.001;

	PxShape* shapes[6];

	for (int i = 0; i < 6; ++i)
	{
		float _halfWidth = (i == 0 || i == 1) ? thickness : halfWidth;
		float _halfHeight = (i == 2 || i == 3) ? thickness : halfHeight;
		float _halfDepth = (i == 4 || i == 5) ? thickness : halfDepth;
		shapes[i] = PhysicsManager::GetInstance()->GetPhysics()->createShape(PxBoxGeometry(_halfWidth, _halfHeight, _halfDepth), *material);
		shapes[i]->setLocalPose(transforms[i]);
	}
	auto owntransform = ConvertPxTransform();

	owntransform.p.x += halfWidth;
	owntransform.p.y += halfHeight + 30.f;
	owntransform.p.z += halfDepth;

	m_rigidStatic = PhysicsManager::GetInstance()->GetPhysics()->createRigidStatic(owntransform);

	if (m_rigidStatic == nullptr)
	{
		LOG_ENGINE("CreateHollowBox Error");
		assert(false);
		return nullptr;
	}

	for (int i = 0; i < 6; ++i) {
		m_rigidStatic->attachShape(*shapes[i]);
		shapes[i]->release();
	}

	SetCollisionFilter(m_rigidStatic);

#ifdef _DEBUG
	m_rigidStatic->setActorFlag(PxActorFlag::eVISUALIZATION, true);
#endif

	material->release();

	PhysicsManager::GetInstance()->AddCollisionOwner(m_rigidStatic, GetOwner());
	return m_rigidStatic;
}

void RigidStaticComponent::Initialize()
{
	PhysicsManager::GetInstance()->AddRigidStaticComponent(this);
	PhysicsManager::GetInstance()->GetScene()->addActor(*m_rigidStatic);
}

void RigidStaticComponent::Update(float deltaTime)
{
}
