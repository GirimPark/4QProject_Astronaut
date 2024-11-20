#pragma once
#include "SceneComponent.h"

class CameraComponent
	: public SceneComponent
{
public:
	Math::Matrix GetViewMatrix() const;
	Math::Matrix GetProjectionMatrix() const;

public:
	void SetFOV(const float& fov) { m_FOV = fov; }
	void SetNearPlane(const float& nearPlane) { m_NearPlane = nearPlane; }
	void SetFarPlane(const float& farPlane) { m_FarPlane = farPlane; }
	void SetAspectRatio(const float& aspectRatio) { m_AspectRatio = aspectRatio; }

	float& GetFOV() { return m_FOV; }
	float& GetNearPlane() { return m_NearPlane; }
	float& GetFarPlane() { return m_FarPlane; }
	float& GetAspectRatio() { return m_AspectRatio; }

public:
	CameraComponent();
	~CameraComponent();

	virtual void Initialize() override;
	virtual void Update(float deltaTime) override;

private:
	float m_FOV;
	float m_NearPlane;
	float m_FarPlane;

	//화면 가로 세로 비율 (Aspect Ratio)
	float m_AspectRatio;
};

