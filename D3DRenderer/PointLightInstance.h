#pragma once

class PointLightInstance
{
public:
	Math::Vector3 m_PointLightPos = { 0, 0, 0 };
	Math::Vector3 m_PointLightColor = { 0, 0, 0 };
	float m_PointLightRange = 300.0f;
	float m_PointLightLinearTerm = 1.0f;
	float m_PointLightQuadraTicTerm = 0.0f;
	float m_PointLightIntencity = 30.0f;
};
