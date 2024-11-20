#pragma once
#include "../Engine/UIClickFunctor.h"

class UIRestartClickFunctor 
    : public UIClickFunctor
{
public:
    ~UIRestartClickFunctor();
    void Execute(UIObject& onwer, float deltaTime) override;
};

