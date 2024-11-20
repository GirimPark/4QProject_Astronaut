#pragma once
#include "../Engine/UIClickFunctor.h"
#include "../Engine/EventListener.h"

class UIGameStartClick 
    :public UIClickFunctor
    , public EventListener
{
public:
    void Execute(UIObject& onwer, float deltaTime) override;

public:
    void HandleEvent(Event* event) override;
};

