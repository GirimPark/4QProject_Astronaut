#include "pch.h"
#include "GridMovementComponent.h"
#include "../Engine/RigidDynamicComponent.h"
#include "../Engine/WorldManager.h"

#include "PlayerObject.h"
#include "TestPlayerObject.h"
#include "PuzzleWorld.h"
#include "TestWorld.h"
#include "ShockParticleObjcect.h"

using namespace physx;

void GridMovementComponent::Initialize()
{
	//m_owner = dynamic_cast<TestPlayerObject*>(GetOwner().lock().get());
	m_owner = dynamic_cast<PlayerObject*>(GetOwner().lock().get());
}

void GridMovementComponent::Update(float deltaTime)
{
	// 1초 뒤에 StartTurn send해주려고
	/*if (m_bSendStartTurn)
	{
		m_sendStartTurnTimer += deltaTime;
		if (m_sendStartTurnTimer >= 1.0f)
		{
			m_owner->Network_ChangeTurn();
			m_sendStartTurnTimer = 0.f;
			m_bSendStartTurn = false;
			WorldManager::GetInstance()->PushSendQueue(
				WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_StartTurn), C2S_START_TURN, nullptr),
				sizeof(PacketC2S_StartTurn));
		}
	}*/

	if (!m_ownerRigidDynamic)
	{
		m_ownerRigidDynamic = m_owner->GetComponentByTypeName<RigidDynamicComponent>().lock()->GetRigidDynamic();
	}

	if (m_isMove)
	{
		MoveOnGrid(deltaTime);
	}
	else if (m_isCrash)
	{
		CrashOnGrid(deltaTime);
	}
	else if (m_isKnockback)
	{
		IsKnockbacking(deltaTime);
	}
	else if (m_isGround)
	{
		IsEndKnockback(deltaTime);
	}

}

void GridMovementComponent::SetMoveStart(const Math::Vector3& targetPosition, bool changeTurn)
{
	m_bChangeTurn = changeTurn;

	if (m_ownerRigidDynamic == nullptr)
	{
		LOG_ENGINE("DynamicRigidBody is Null");
		assert(false);
		return;
	}

	m_startPosition = m_ownerRigidDynamic->getGlobalPose().p;
	m_startRotation = m_ownerRigidDynamic->getGlobalPose().q;

	m_targetPosition.x = targetPosition.x;
	m_targetPosition.y = targetPosition.y;
	m_targetPosition.z = targetPosition.z;

	auto direction = m_targetPosition - m_startPosition;

	if (direction.x != 0.0f || direction.z != 0.0f)
	{
		m_targetRotation = PxQuat(-(std::atan2(direction.x, -direction.z)), PxVec3(0.f, 1.f, 0.f));
	}
	else
	{
		m_targetRotation = m_startRotation;
	}

	if (m_isGround)
	{
		m_movementSpeed = 1.f;
		m_rotationSpeed = 0.5f;
		m_isGround = false;
	}
	else
	{
		m_movementSpeed = 1.0f;
		m_rotationSpeed = 0.5f;
	}

	m_isMove = true;
}

void GridMovementComponent::MoveOnGrid(float deltaTime)
{
	m_elapsedTime += deltaTime;

	float moveInterpolationFactor = std::min<float>(m_elapsedTime / m_movementSpeed, 1.0f);
	float rotationInterpolationFactor = std::min<float>(m_elapsedTime / m_rotationSpeed, 1.0f);

	PxVec3 interpolatedPosition;
	interpolatedPosition.x = Interpolation::PxLerp(m_startPosition.x, m_targetPosition.x, moveInterpolationFactor);
	interpolatedPosition.y = Interpolation::PxLerp(m_startPosition.y, m_targetPosition.y, moveInterpolationFactor);
	interpolatedPosition.z = Interpolation::PxLerp(m_startPosition.z, m_targetPosition.z, moveInterpolationFactor);

	auto interpolatedRotation = PxSlerp(rotationInterpolationFactor, m_startRotation, m_targetRotation);

	PxTransform ownerTransform(interpolatedPosition, interpolatedRotation);

	m_ownerRigidDynamic->setGlobalPose(ownerTransform);

	if (m_childRigidDynamic)
	{
		PxTransform childTransform = m_childRigidDynamic->getGlobalPose();

		PxVec3 movementDifference = interpolatedPosition - m_previousPosition;
		PxVec3 newPosition = childTransform.p + movementDifference;

		PxQuat rotationDifference = interpolatedRotation * m_previousRotation.getConjugate();
		PxQuat newRotation = rotationDifference * childTransform.q;

		m_childRigidDynamic->setGlobalPose(PxTransform(newPosition, newRotation));
	}

	if (moveInterpolationFactor >= 1.0f && rotationInterpolationFactor >= 1.0f)
	{
		m_isMove = false;
		m_isCrashing = false;
		m_elapsedTime = 0.f;

		m_finalPosition = m_ownerRigidDynamic->getGlobalPose().p;

		m_owner->MovementFinished();

		if (m_owner->GetTurn())
		{
			if (m_bChangeTurn)
			{
				std::shared_ptr<PuzzleWorld> pPuzzleWorld = std::dynamic_pointer_cast<PuzzleWorld>(m_owner->GetOwnerWorld().lock());
				m_owner->ChangeFinishedActionUI();
				m_owner->InCreaseIndex();
				// 행동 남아있는지 체크
				if (pPuzzleWorld->CheckRemainingAction())
				{
					// 남아있으면 1초뒤에 서버에 StartTurn send 시키기
					//m_bSendStartTurn = true;
					m_owner->SetWaitingChange(true);
					/*PacketC2S_MovementEnd me;
					if (m_owner->GetHostPlayer())
						me.who = '0';
					else
						me.who = '1';
					WorldManager::GetInstance()->PushSendQueue(
						WorldManager::GetInstance()->SerializeBuffer(sizeof(PacketC2S_MovementEnd), C2S_MOVEMENT_END, &me.who),
						sizeof(PacketC2S_MovementEnd));*/
				}
				m_bChangeTurn = false;
			}
		}
	}

	m_previousPosition = interpolatedPosition;
	m_previousRotation = interpolatedRotation;
}

void GridMovementComponent::CrashOnGrid(float deltaTime)
{
	m_isCrashing = true;

	physx::PxVec3 direction;
	if (m_owner->GetTurn())
	{
		direction = m_startPosition - m_targetPosition;
		direction.normalize();

		direction.x = std::round(direction.x);
		direction.y = std::round(direction.y);
		direction.z = std::round(direction.z);
	}
	else
	{
		direction = m_ownerRigidDynamic->getGlobalPose().p - m_finalPosition;
		direction.normalize();

		direction.x = std::round(direction.x);
		direction.y = std::round(direction.y);
		direction.z = std::round(direction.z);
	}

	m_ownerRigidDynamic->addForce(direction * 120000.f, PxForceMode::eIMPULSE);
	m_isCrash = false;
	m_isKnockback = true;
}

void GridMovementComponent::IsKnockbacking(float deltaTime)
{
	PxVec3 linearVelocity = m_ownerRigidDynamic->getLinearVelocity();

	if (linearVelocity.magnitudeSquared() < 0.1f)
	{
		m_ownerRigidDynamic->clearForce();
		m_ownerRigidDynamic->putToSleep();
		m_isGround = true;
		m_isKnockback = false;
	}
	//auto curPosition = m_ownerRigidDynamic->getGlobalPose().p;
	//PxVec3 diffPosition;
	//if (m_owner->GetTurn())
	//	diffPosition = m_startPosition - curPosition;
	//else
	//	diffPosition = m_finalPosition - curPosition;
	//auto diffNormal = diffPosition.getNormalized();
	//diffPosition += diffNormal;
	//diffPosition.abs();

	//if (diffPosition.x <= 1.f &&
	//	diffPosition.y <= 1.f &&
	//	diffPosition.z <= 1.f)
	//{
	//	m_ownerRigidDynamic->clearForce();
	//	m_ownerRigidDynamic->putToSleep();
	//}
	else
	{
		PxVec3 dampingForce = -linearVelocity * m_dampingFactor;

		m_ownerRigidDynamic->addForce(dampingForce, PxForceMode::eFORCE);
	}
}

void GridMovementComponent::InitGridMovement()
{
	m_isMove = false;
	m_isCrash = false;
	m_isCrashing = false;
	m_isKnockback = false;
	m_isGround = false;
	m_bChangeTurn = false;
}

void GridMovementComponent::IsEndKnockback(float deltaTime)
{
	Math::Vector3 previousPosition;

	if (m_owner->GetTurn())
	{
		previousPosition.x = m_startPosition.x;
		previousPosition.y = m_startPosition.y;
		previousPosition.z = m_startPosition.z;
	}
	else
	{
		previousPosition.x = m_finalPosition.x;
		previousPosition.y = m_finalPosition.y;
		previousPosition.z = m_finalPosition.z;
	}
	SetMoveStart(previousPosition, true);

}



