#pragma once
#include "../Engine/UIClickFunctor.h"

class UITutorialNextClick
    : public UIClickFunctor
{
public:
    virtual void Execute(UIObject& onwer, float deltaTime) override;

    void SetCount(int* count) { m_Count = count; }

public:
    int* m_Count;
};

