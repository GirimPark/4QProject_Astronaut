#pragma once
#include "../Engine/RenderComponent.h"
#include "../D3DRenderer/PointLightInstance.h"

class PointLightComponent
	: public RenderComponent
{
public:
	PointLightComponent();
	~PointLightComponent() override;

public:
	std::shared_ptr<PointLightInstance> GetPointLight() const { return  m_pointLight; }

public:
	void Initialize() override;	// PointLightInstance을 렌더러를 통해 생성, 받아온다.
	void Update(float deltaTime) override;
	//void SetWorldMatrix(Math::Matrix* worldMatrix) { m_worldMatrix = *(worldMatrix); }

	void SetRange(float _range) { m_pointLight->m_PointLightRange = _range; }
	void SetPointLightLinearTerm(float _linearTerm) { m_pointLight->m_PointLightLinearTerm = _linearTerm; }
	void SetPointLightQuadraTicTerm(float _quadraTicTerm) { m_pointLight->m_PointLightQuadraTicTerm = _quadraTicTerm; }
	void SetPointLightIntencity(float _intencity) { m_pointLight->m_PointLightIntencity  = _intencity; }
	void SetLightColor(float _r, float _g, float _b);

private:
	std::shared_ptr<PointLightInstance> m_pointLight;	
};
