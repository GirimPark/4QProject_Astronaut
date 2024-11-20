#pragma once
#include "../Engine/UIClickPopUpFunctor.h"

class PlayerObject;

class UIReadyClickPopUpFunctor 
    : public UIClickPopUpFunctor
{
public:
    // UIClickFunctor을(를) 통해 상속됨
    void Execute(UIObject& onwer, float deltaTime) override;

    void SetPlayer(std::shared_ptr<PlayerObject> player) { m_player = player; }

private:
    std::weak_ptr<PlayerObject> m_player;
};