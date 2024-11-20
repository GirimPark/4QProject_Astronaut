#include "pch.h"
#include "GameObject.h"

#include "SceneComponent.h"

GameObject::~GameObject()
{
	for(auto& component : m_ownComponents)
	{
		component.reset();
	}
	m_ownComponents.clear();
}

void GameObject::SetPosition(Vector3 position)
{
	m_pRootComponent->SetLocalPosition(position);
}

void GameObject::SetRotation(Vector3 rotation)
{
	m_pRootComponent->SetLocalRotation(rotation);
}

void GameObject::SetScale(Vector3 scale)
{
	m_pRootComponent->SetLocalScale(scale);
}

std::weak_ptr<Component> GameObject::GetComponent(std::string_view name) const
{
	return m_ownComponents[m_componentMap.find(std::string(name))->second];
}

const Vector3& GameObject::GetLocalPosition() const
{
	return m_pRootComponent->GetLocalPosition();
}

const Vector3& GameObject::GetLocalRotation() const
{
	return m_pRootComponent->GetLocalRotation();
}

const Vector3& GameObject::GetLocalScale() const
{
	return m_pRootComponent->GetLocalScale();
}

const Vector3& GameObject::GetWorldPosition() const
{
	return m_pRootComponent->GetWorldPosition();
}

const Vector3& GameObject::GetWorldRotation() const
{
	return m_pRootComponent->GetWorldRotation();
}

const Vector3& GameObject::GetWorldScale() const
{
	return m_pRootComponent->GetWorldScale();
}

const Matrix& GameObject::GetTransform() const
{
	return m_pRootComponent->GetWorldTransform();
}

void GameObject::Initialize()
{
	for (const auto& component : m_ownComponents)
	{
		component->Initialize();
	}
}

void GameObject::Update(float deltaTime)
{
	assert(m_pRootComponent);

	for (const auto& component : m_ownComponents)
	{
		//if (component == m_pRootComponent)
		//	continue;
		component->Update(deltaTime);
	}
	m_pRootComponent->Update(deltaTime);
}