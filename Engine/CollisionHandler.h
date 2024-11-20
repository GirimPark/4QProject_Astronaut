#pragma once
#include "PxPhysicsAPI.h"
/// <summary>
/// CollisionHandler
/// 충돌 응답 구현하는 순수 가상 클래스
/// 이거 상속시킨 클래스에서 각 함수 구현 해주고
/// 리지드바디 컴포넌트에서 AddCollisionHandler로 붙여주면 됨.
/// </summary>
class GameObject;

class CollisionHandler
{
public:
	virtual ~CollisionHandler() {}

	virtual void OnContactEnter(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp) abstract;
	virtual void OnContactStay(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp) abstract;
	virtual void OnContactExit(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxContactPair& cp) abstract;

	virtual void OnTriggerEnter(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxTriggerPair& tp) abstract;
	virtual void OnTriggerExit(std::weak_ptr<GameObject> actor1, std::weak_ptr<GameObject> actor2, const physx::PxTriggerPair& tp) abstract;


};

