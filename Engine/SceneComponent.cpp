#include "pch.h"
#include "SceneComponent.h"

#include "GameObject.h"

void SceneComponent::SetLocalPosition(const Math::Vector3 pos)
{
	m_bCalced = false;
	m_localPosition = pos;
}

void SceneComponent::SetLocalScale(const Math::Vector3 scale)
{
	m_bCalced = false;
	m_localScale = scale;
}

void SceneComponent::SetLocalRotation(const Math::Vector3 rot)
{
	m_bCalced = false;
	m_localRotation = rot;
}

void SceneComponent::SetLocalTransform(Math::Matrix matrix)
{
	m_bCalced = false;
	Math::Quaternion q;
	matrix.Decompose(m_localScale, q, m_localPosition);
	m_localRotation = q.ToEuler();
}

void SceneComponent::Initialize()
{
	Transform::m_owner = Component::m_pOwner.lock().get();
}

void SceneComponent::Update(float deltaTime)
{
	if (!m_bCalced)
	{
		m_localMatrix = Math::Matrix::CreateScale(m_localScale) *
			Math::Matrix::CreateFromYawPitchRoll(XMConvertToRadians(m_localRotation.y), XMConvertToRadians(m_localRotation.x), XMConvertToRadians(m_localRotation.z)) *
			Math::Matrix::CreateTranslation(m_localPosition);

		m_bCalced = true;
	}

	//m_pParent = m_pOwner.lock()->GetRootComponent().lock().get();

	std::shared_ptr<GameObject> parentObj = GetOwner().lock()->GetParentObject().lock();
	if (parentObj)
	{
		m_pParent = parentObj->GetRootComponent().lock().get();
	}
	else
	{
		m_pParent = nullptr;
	}

	Transform::Update(deltaTime);

	Quaternion rotationQuat;
	m_worldMatrix.Decompose(m_worldScale, rotationQuat, m_worldPosition);
	m_worldRotation = { XMConvertToDegrees(rotationQuat.ToEuler().x), XMConvertToDegrees(rotationQuat.ToEuler().y), XMConvertToDegrees(rotationQuat.ToEuler().z) };
}