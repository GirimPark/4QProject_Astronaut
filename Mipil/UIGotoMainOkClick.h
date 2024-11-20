#pragma once
#include "../Engine/UIClickFunctor.h"
#include "../Engine/EventListener.h"

class UIGotoMainOkClick
    : public UIClickFunctor
	, public EventListener
{
public:
	~UIGotoMainOkClick() = default;
	void Execute(UIObject& onwer, float deltaTime) override;

	void HandleEvent(Event* event) override;
};

