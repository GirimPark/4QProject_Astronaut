#pragma once
#include "../Engine/UIClickFunctor.h"

class UIEndGameOkClick 
    : public UIClickFunctor
{
public:
    virtual void Execute(UIObject& onwer, float deltaTime) override;
};

