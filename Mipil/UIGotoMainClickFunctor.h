#pragma once
#include "../Engine/UIClickFunctor.h"

class UIGotoMainClickFunctor 
    : public UIClickFunctor
{
public:
	~UIGotoMainClickFunctor() = default;
	void Execute(UIObject& onwer, float deltaTime) override;
};

