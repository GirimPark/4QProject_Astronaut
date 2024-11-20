#pragma once
#include "../Engine/GameObject.h"

class PlayerFreeCamera;
class FireParticleObject;

class PlayerFreeObject :
    public GameObject
{
public:
    void SetCamera(PlayerFreeCamera* cam) { m_myCam = cam; }

    PlayerFreeCamera* GetCamera() { return m_myCam; }

public:
    PlayerFreeObject();
    ~PlayerFreeObject();

public:
    void SetFireParticle(std::shared_ptr<FireParticleObject> fireParticle) { m_fireParticleObject = fireParticle; }
    void Fire(bool isPlay);

    void Initialize() override;
    void Update(float deltaTime) override;

private:
    PlayerFreeCamera* m_myCam;
    std::shared_ptr<FireParticleObject> m_fireParticleObject;
};

