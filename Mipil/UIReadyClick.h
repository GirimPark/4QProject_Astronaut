#pragma once
#include "../Engine/UIClickFunctor.h"

class PlayerObject;

class UIReadyClick 
    : public UIClickFunctor
{
public:
    virtual void Execute(UIObject& onwer, float deltaTime) override;

    void SetPlayer(std::shared_ptr<PlayerObject> player) { m_player = player; }

public:
    std::weak_ptr<PlayerObject> m_player;
};

