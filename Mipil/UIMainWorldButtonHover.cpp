#include "pch.h"
#include "UIMainWorldButtonHover.h"
#include "../Engine/UIObject.h"

void UIMainWorldButtonHover::ExecuteHovered(UIObject& onwer, float deltaTime)
{
	m_hovering = true;
	//LOG_ENGINE("GameStart Hover In UI");
	onwer.GetUIInstance().lock()->LoadTextureResource(m_hoverTexturePath);
}

void UIMainWorldButtonHover::ExecuteHoverOut(UIObject& onwer, float deltaTime)
{
	if(m_hovering)
	{
		//LOG_ENGINE("GameStart Hover Out UI");
		onwer.GetUIInstance().lock()->LoadTextureResource(onwer.GetTexturePath());
		m_hovering = false;
	}
}
