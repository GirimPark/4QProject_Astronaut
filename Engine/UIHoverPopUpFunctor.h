#pragma once
#include "UIHoverFunctor.h"

class UIObject;

class UIHoverPopUpFunctor :
    public UIHoverFunctor
{
public:
	virtual void ExecuteHovered(UIObject& onwer, float deltaTime) override;
	virtual void ExecuteHoverOut(UIObject& onwer, float deltaTime) override;

    void SetTargetUI(std::weak_ptr<UIObject> targetUI);

private:
    std::weak_ptr<UIObject> m_TargetUI;
};