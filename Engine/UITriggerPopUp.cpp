#include "pch.h"
#include "UITriggerPopUp.h"

UITriggerPopUp::UITriggerPopUp(const std::wstring& name)
	: UIObject(name)
	, m_triggerPop(false)
{
}

UITriggerPopUp::~UITriggerPopUp()
{
}

void UITriggerPopUp::Initialize()
{
	__super::Initialize();
	m_bVisible = false;

	for (auto& child : m_children)
	{
		child->SetVisible(false);
	}
}

void UITriggerPopUp::Update(float deltaTime)
{
	__super::Update(deltaTime);

	if (m_triggerPop == false && m_bVisible == true)
	{
		m_bVisible = false;

		for (auto& child : m_children)
		{
			child->SetVisible(false);
		}
	}

	if (m_triggerPop == true && m_bVisible == false)
	{
		m_bVisible = true;

		for (auto& child : m_children)
		{
			child->SetVisible(true);
		}
	}
}

void UITriggerPopUp::TriggerPop()
{
	m_triggerPop = !m_triggerPop;
}