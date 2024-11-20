#pragma once
#include "../Engine/Controller.h"
#include "PxPhysicsAPI.h"
#include <unordered_set>
#include <directXTK/Keyboard.h>

class PlayerFreeObject;

class PlayerMovementContorller :
    public Controller
{
public:
    // Controller을(를) 통해 상속됨
    void HandleInput(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;
    void Update(float deltaTime) override;

private:
    physx::PxRigidDynamic* m_rigidDynamic;

    PlayerFreeObject* m_playerObject;

    bool m_isPushedButton = false;
    bool m_isFire = false;

    Math::Vector3 m_playerForward;
    Math::Vector3 m_playerRight;

    float m_moveSpeed = 2.5f;
    float m_dampingFactor = 10000.f;

    float m_pitch = 0.f;
    float m_roll = 0.f;
    float m_yaw = 0.f;

    float m_smoothingFactor = 0.05f;

};

