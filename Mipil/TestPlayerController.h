#pragma once
#include "../Engine/Controller.h"
#include "PxPhysicsAPI.h"

class TestWorld;
class TestPlayerObject;

class TestPlayerController :
    public Controller
{
public:
    // Controller을(를) 통해 상속됨
    void HandleInput(const Keyboard::State& KeyState, const Keyboard::KeyboardStateTracker& KeyTracker, const Mouse::State& MouseState, const Mouse::ButtonStateTracker& MouseTracker) override;
    void Update(float deltaTime) override;

private:
    TestWorld* m_world;
    TestPlayerObject* m_player;
    physx::PxRigidDynamic* m_rigidDynamic;
};

