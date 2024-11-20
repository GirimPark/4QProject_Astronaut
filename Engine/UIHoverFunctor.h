#pragma once
#include "UIFunctor.h"

class UIHoverFunctor :
	public UIFunctor
{
public:
	virtual ~UIHoverFunctor() = default;
	virtual void ExecuteHovered(UIObject& onwer, float deltaTime) abstract;
	virtual void ExecuteHoverOut(UIObject& onwer, float deltaTime) abstract;
};