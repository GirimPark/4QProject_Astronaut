#include "pch.h"
#include "UIText.h"
#include "../Engine/CommonApp.h"

void UIText::Update(float deltaTime)
{
	__super::Update(deltaTime);

	if (m_bVisible)
	{
		CommonApp::m_pInstance->GetRenderer()->DrawTextRequest(m_text, m_worldMatrix._41, -m_worldMatrix._42, m_color, m_fontFamily, m_fontSize, m_fontWeight, m_fontStyle, m_fontStretch);
	}
}
