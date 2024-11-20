#include "pch.h"
#include "PhysicsManager.h"
#include "RigidDynamicComponent.h"
#include "RigidStaticComponent.h"
#include "SceneComponent.h"
#include "GameObject.h"

#ifdef _DEBUG
#include "../Engine/CommonApp.h"
#include "../D3DRenderer/D3DRenderManager.h"
#endif

#define PVD_HOST "127.0.0.1"

using namespace physx;

PhysicsManager::~PhysicsManager()
{
}

void PhysicsManager::AddRigidDynamicComponent(RigidDynamicComponent* rigidComponents)
{
	m_rigidDynamicComponents.push_back(rigidComponents);
}

void PhysicsManager::RemoveRigidDynamicComponent(RigidDynamicComponent* rigidComponents)
{
	m_rigidDynamicComponents.remove(rigidComponents);
}

void PhysicsManager::AddRigidStaticComponent(RigidStaticComponent* rigidComponents)
{
	m_rigidStaticComponents.push_back(rigidComponents);
}

void PhysicsManager::RemoveRigidStaticComponent(RigidStaticComponent* rigidComponents)
{
	m_rigidStaticComponents.remove(rigidComponents);
}

void PhysicsManager::RemoveCollisionHandler(physx::PxActor* actor)
{
	auto it = m_collisionHandlers.find(actor);
	if (it != m_collisionHandlers.end())
	{
		m_collisionHandlers.erase(it);
	}
}

void PhysicsManager::RemoveCollisionOwner(physx::PxActor* actor)
{
	auto it = m_collisionOwner.find(actor);
	if (it != m_collisionOwner.end())
	{
		m_collisionOwner.erase(it);
	}
}

std::weak_ptr<CollisionHandler> PhysicsManager::GetCollisionHandler(physx::PxActor* actor)
{
	auto it = m_collisionHandlers.find(actor);
	if (it != m_collisionHandlers.end()) {
		return it->second;
	}
	return std::weak_ptr<CollisionHandler>();
}

std::weak_ptr<GameObject> PhysicsManager::GetCollisionOwner(physx::PxActor* actor)
{
	auto it = m_collisionOwner.find(actor);
	if (it != m_collisionOwner.end()) {
		return it->second;
	}
	return std::weak_ptr<GameObject>();
}

void PhysicsManager::Initailize()
{
	m_foundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);

	/*m_pvd = PxCreatePvd(*m_foundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	m_pvd->connect(*transport, PxPvdInstrumentationFlag::eALL);*/

	m_physics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), true, m_pvd);

	PxSceneDesc sceneDesc(m_physics->getTolerancesScale());
	//sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
	sceneDesc.gravity = PxVec3(0.0f, .0f, 0.0f);
	m_dispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = m_dispatcher;
	//sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.filterShader = m_CollisionFilter->FilterShader;
	m_CollisionEventCallback = std::make_unique<CollisionEventCallback>();
	sceneDesc.simulationEventCallback = m_CollisionEventCallback.get();
	m_scene = m_physics->createScene(sceneDesc);

	//DebugVisualization
#ifdef _DEBUG
	m_scene->setVisualizationParameter(PxVisualizationParameter::eSCALE, 1.0f);
	m_scene->setVisualizationParameter(PxVisualizationParameter::eWORLD_AXES, INT_MAX);
	//m_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_AXES, 1.0f);
	//m_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_MASS_AXES, 1.0f);
	m_scene->setVisualizationParameter(PxVisualizationParameter::eBODY_LIN_VELOCITY, .1f);
	m_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_SHAPES, 1.0f);
	//m_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_STATIC, 1.0f);
	//m_scene->setVisualizationParameter(PxVisualizationParameter::eCOLLISION_DYNAMIC, 1.0f);
	//m_scene->setVisualizationCullingBox(PxBounds3({ -1000, -400, -1000 }, { 1000, 400, 1000 }));
#endif
	//PxPvdSceneClient* pvdClient = m_scene->getScenePvdClient();
	//if (pvdClient)
	//{
	//	pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
	//	pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
	//	pvdClient->setScenePvdFlag(PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
	//}
}

void PhysicsManager::Finalize()
{
	PX_RELEASE(m_scene);
	PX_RELEASE(m_dispatcher);
	PX_RELEASE(m_physics);
	if (m_pvd) {
		PxPvdTransport* transport = m_pvd->getTransport();
		m_pvd->release();
		PX_RELEASE(transport);
	}
	PX_RELEASE(m_foundation);

	m_rigidDynamicComponents.clear();
	m_rigidStaticComponents.clear();
	m_collisionHandlers.clear();
	m_collisionOwner.clear();

	delete m_pInstance;
}

void PhysicsManager::Update(float deltaTime)
{
	StepPhysics(deltaTime);

	for (auto& it : m_rigidDynamicComponents)
	{
		it->Update(deltaTime);
	}

//#ifdef _DEBUG
//	const PxRenderBuffer& rb = m_scene->getRenderBuffer();
//
//	std::vector<DebugPoint> debugPoints;
//	std::vector<DebugLine> debugLines;
//
//	for (PxU32 i = 0; i < rb.getNbPoints(); i++)
//	{
//		const PxDebugPoint& point = rb.getPoints()[i];
//		// render the point
//		DebugPoint vertex;
//		vertex.m_Position.x = point.pos.x;
//		vertex.m_Position.y = point.pos.y;
//		vertex.m_Position.z = point.pos.z;
//		vertex.m_Color.x = ((point.color >> 24) & 0xFF) / 255.0f;
//		vertex.m_Color.y = ((point.color >> 16) & 0xFF) / 255.0f;
//		vertex.m_Color.z = ((point.color >> 8) & 0xFF) / 255.0f;
//		vertex.m_Color.w = 1.f;
//		debugPoints.push_back(vertex);
//	}
//
//	for (PxU32 i = 0; i < rb.getNbLines(); i++)
//	{
//		const PxDebugLine& line = rb.getLines()[i];
//		// render the line
//		DebugLine line_;
//		line_.m_StartLine.x = line.pos0.x;
//		line_.m_StartLine.y = line.pos0.y;
//		line_.m_StartLine.z = line.pos0.z;
//		line_.m_EndLine.x = line.pos1.x;
//		line_.m_EndLine.y = line.pos1.y;
//		line_.m_EndLine.z = line.pos1.z;
//		line_.m_StartColor.x = ((line.color0 >> 24) & 0xFF) / 255.0f;
//		line_.m_StartColor.y = ((line.color0 >> 16) & 0xFF) / 255.0f;
//		line_.m_StartColor.z = ((line.color0 >> 8) & 0xFF) / 255.0f;
//		line_.m_StartColor.w = 1.f;
//		line_.m_EndColor.x = ((line.color1 >> 24) & 0xFF) / 255.0f;
//		line_.m_EndColor.y = ((line.color1 >> 16) & 0xFF) / 255.0f;
//		line_.m_EndColor.z = ((line.color1 >> 8) & 0xFF) / 255.0f;
//		line_.m_EndColor.w = 1.f;
//		debugLines.push_back(line_);
//	}
//	CommonApp::m_pInstance->GetRenderer()->AddDebugPoint(debugPoints);
//	CommonApp::m_pInstance->GetRenderer()->AddDebugLine(debugLines);
//#endif
}

void PhysicsManager::StepPhysics(float deltaTime)
{
	m_accumulator += deltaTime * 5;
	if (m_accumulator < m_stepSize)
		return;

	m_accumulator -= m_stepSize;

	m_scene->simulate(m_stepSize);
	m_scene->fetchResults(true);
}
