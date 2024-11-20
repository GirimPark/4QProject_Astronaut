#pragma once
#include "../Engine/UIFunctor.h"
class UITimerFunctor :
    public UIFunctor
{
public:
    virtual ~UITimerFunctor() = default;
    void SetTargetUI(std::weak_ptr<UIObject> targetUI);

private:
    std::weak_ptr<UIObject> m_targetUI;
};

