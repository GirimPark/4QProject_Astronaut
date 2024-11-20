#pragma once
#include "../Engine/UIClickFunctor.h"
#include "../Engine/EventListener.h"

class UINoClick 
    : public UIClickFunctor
    , public EventListener
{
public:
    virtual void Execute(UIObject& onwer, float deltaTime) override;

public:
    void HandleEvent(Event* event) override;
};
