#include "pch.h"
#include "UIClickPopUpFunctor.h"
#include "UIObject.h"

void UIClickPopUpFunctor::Execute(UIObject& onwer, float deltaTime)
{
	__super::Execute(onwer, deltaTime);

	m_TargetUI.lock()->SetVisible(true);

	auto& targetChildren = m_TargetUI.lock()->GetChildren();

	for (auto& child : targetChildren)
	{
		child->SetVisible(true);
	}
	
}

void UIClickPopUpFunctor::SetTargetUI(std::weak_ptr<UIObject> targetUI)
{
	m_TargetUI = targetUI;
}