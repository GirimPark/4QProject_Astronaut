#include "pch.h"
#include "UIHoverPopUpFunctor.h"
#include "UIObject.h"

void UIHoverPopUpFunctor::ExecuteHovered(UIObject& onwer, float deltaTime)
{
	m_TargetUI.lock()->SetVisible(true);

	auto& targetChildren = m_TargetUI.lock()->GetChildren();

	for (auto& child : targetChildren)
	{
		child->SetVisible(true);
	}
}

void UIHoverPopUpFunctor::ExecuteHoverOut(UIObject& onwer, float deltaTime)
{
	m_TargetUI.lock()->SetVisible(false);

	auto& targetChildren = m_TargetUI.lock()->GetChildren();

	for (auto& child : targetChildren)
	{
		child->SetVisible(false);
	}
}

void UIHoverPopUpFunctor::SetTargetUI(std::weak_ptr<UIObject> targetUI)
{
	m_TargetUI = targetUI;
}
