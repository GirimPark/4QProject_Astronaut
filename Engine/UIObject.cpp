#include "pch.h"
#include "UIObject.h"
#include "InputManager.h"
#include "CommonApp.h"

#include "../D3DRenderer/UIInstance.h"

UIObject::UIObject(const std::wstring& name)
	: m_name(name)
	, m_bVisible(true)
	, m_position(Math::Vector3::Zero)
	, m_scale(Math::Vector3::One)
{
}

UIObject::~UIObject()
{
}

void UIObject::ExecuteHoverFunctors(float deltaTime)
{
	for (auto& functor : m_mouseHoverFunctors)
	{
		functor->ExecuteHovered(*this, deltaTime);
	}
}

void UIObject::ExecuteHoverOutFunctors(float deltaTime)
{
	for (auto& functor : m_mouseHoverFunctors)
	{
		functor->ExecuteHoverOut(*this, deltaTime);
	}
}

void UIObject::ExecuteClickFunctors(float deltaTime)
{
	for (auto& functor : m_mouseClickFunctors)
	{
		functor->Execute(*this, deltaTime);
	}
}

void UIObject::Initialize()
{
	m_uiInstance = std::make_shared<UIInstance>();
	m_uiInstance->LoadTextureResource(m_texturePath);
	m_uiInstance->Create(&m_worldMatrix, m_size);
}

void UIObject::Update(float deltaTime)
{
	m_uiInstance->SetIsRender(m_bVisible);
	m_worldMatrix = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 1.f) * Math::Matrix::CreateTranslation(m_position.x, m_position.y, 0.f);

	if (m_parent.lock())
	{
		m_worldMatrix = m_worldMatrix * m_parent.lock()->GetWorldMatrix();
	}

	if (m_bVisible) 
	{
		CommonApp::m_pInstance->GetRenderer()->AddUIInstance(m_uiInstance);
	}

	for (auto& child : m_children)
	{
		child->Update(deltaTime);
	}
}


void UIObject::SetParent(std::shared_ptr<UIObject> parent)
{
	m_parent = parent;
}

void UIObject::RemoveChild(std::shared_ptr<UIObject> child)
{
	auto it = std::find(m_children.begin(), m_children.end(), child);

	if (it != m_children.end())
	{
		m_children.erase(it);
	}
}

void UIObject::SetAllVisible(bool isVisible)
{ 
	m_bVisible = isVisible;

	for (auto& child : m_children)
	{
		child->SetAllVisible(isVisible);
	}
}