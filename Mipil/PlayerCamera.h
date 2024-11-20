#pragma once
#include "../Engine/GameObject.h"

class PlayerCamera
	: public GameObject
{
public:
	void SetSubject(std::weak_ptr<GameObject> subject) { m_subject = subject.lock().get(); }
	void SetDistance(float distance) { m_distance = m_distance; }
	void SetRotationSpeed(float rotationSpeed) { m_rotationSpeed = m_rotationSpeed; }
	void SetLookAtOffset(const Math::Vector3& lookAtOffset){ m_lookAtOffset = m_lookAtOffset; }

	GameObject* GetSubject() { return m_subject; }

public:
	void Initialize() override;
	void Update(float deltaTime) override;

private:
	GameObject* m_subject;

	float m_distance = 1000.0f;
	//-89.9f ~ 89.9f
	float m_minPitch = -6.f;
	float m_maxPitch = 70.f;

	float m_rotationSpeed = 0.2f;
	Math::Vector3 m_lookAtOffset = Math::Vector3(0.0f, 0.0f, 0.0f);

	friend class PlayerCameraController;
};