#include "pch.h"
#include "UIEndGameClick.h"
#include "../Engine/UIObject.h"

void UIEndGameClick::Execute(UIObject& onwer, float deltaTime)
{
	__super::Execute(onwer, deltaTime);

	m_targetUI.lock()->SetVisible(true);

	auto& targetChildren = m_targetUI.lock()->GetChildren();

	for (auto& child : targetChildren)
	{
		child->SetVisible(true);
	}
}

void UIEndGameClick::SetTargetUI(std::weak_ptr<UIObject> targetUI)
{
	m_targetUI = targetUI;
}
