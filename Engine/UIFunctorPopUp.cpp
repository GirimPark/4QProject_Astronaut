#include "pch.h"
#include "UIFunctorPopUp.h"

UIFunctorPopUp::UIFunctorPopUp(const std::wstring& name)
	: UIObject(name)
{
}

UIFunctorPopUp::~UIFunctorPopUp()
{
}
 
void UIFunctorPopUp::Initialize()
{
	__super::Initialize();
	m_bVisible = false;

	for (auto& child : m_children)
	{
		child->SetVisible(false);
	}
}

void UIFunctorPopUp::Update(float deltaTime)
{
	__super::Update(deltaTime);
}
