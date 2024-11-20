#pragma once
#include "../Engine/Component.h"
#include "PxPhysicsAPI.h"

class PlayerObject;
class TestPlayerObject;
class ShockParticleObjcect;

class GridMovementComponent :
	public Component
{
public:
	void SetIsMove(bool isMove) { m_isMove = isMove; }
	void SetIsCrash(bool isCrash) { m_isCrash = isCrash; }
	void SetIsGround(bool isGround) { m_isGround = isGround; }
	void SetIsKnockback(bool isKnockback) { m_isKnockback = isKnockback; }

	void SetMovementSpeed(float speed) { m_movementSpeed = speed; }
	void SetRotationSpeed(float speed) { m_rotationSpeed = speed; }
	void SetChildRigidBody(physx::PxRigidDynamic* child) { m_childRigidDynamic = child; };

	bool GetIsMove() { return m_isMove;	}
	bool GetIsCrashing() { return m_isCrashing;	}
	bool GetIsKnockback() { return m_isKnockback; }

public:
	void SetMoveStart(const Math::Vector3& targetPosition, bool changeTurn);

	void InitGridMovement();

public:
	void Initialize() override;
	void Update(float deltaTime) override;

private:
	void MoveOnGrid(float deltaTime);
	void CrashOnGrid(float deltaTime);
	void IsEndKnockback(float deltaTime);
	void IsKnockbacking(float deltaTime);

private:
	bool m_isMove = false;
	bool m_isCrash = false;
	bool m_isCrashing = false;
	bool m_isKnockback = false;
	bool m_isGround = false;
	bool m_bChangeTurn = false;
//	bool m_bSendStartTurn = false;

	physx::PxVec3 m_startPosition;
	physx::PxVec3 m_targetPosition;
	physx::PxQuat m_startRotation;
	physx::PxQuat m_targetRotation;
	physx::PxVec3 m_previousPosition;
	physx::PxVec3 m_finalPosition;
	physx::PxQuat m_previousRotation;
	physx::PxQuat m_yawPitchAngle;

	float m_sendStartTurnTimer = 0.0f;
	float m_dampingFactor = 3000.f;
	float m_movementSpeed = 1.0f;
	float m_rotationSpeed = 0.5f;

	float m_elapsedTime;

	physx::PxRigidDynamic* m_ownerRigidDynamic;

	physx::PxRigidDynamic* m_childRigidDynamic;

	PlayerObject* m_owner;
	//TestPlayerObject* m_owner;
};

