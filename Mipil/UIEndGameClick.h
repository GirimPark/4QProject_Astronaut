#pragma once
#include "../Engine/UIClickFunctor.h"

class UIObject;

class UIEndGameClick 
    : public UIClickFunctor
{
public:
    virtual void Execute(UIObject& onwer, float deltaTime) override;

    void SetTargetUI(std::weak_ptr<UIObject> targetUI);

private:
    std::weak_ptr<UIObject> m_targetUI;
};

