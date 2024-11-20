#pragma once

struct PipeLineInfo
{
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;
};

class PipeLine
{
public:
	PipeLine();
	~PipeLine();

public:
	ComPtr<ID3D11BlendState> GetAlphaBlendState() { return m_pAlphaBlendState; }
	ComPtr<ID3D11BlendState> GetParticleBlendState() { return m_pParticleBlendState; }
	ComPtr<ID3D11DepthStencilState> GetLessEqualDSS() { return m_LessEqualDSS; }
	ComPtr<ID3D11DepthStencilState> GetNoDepthWritesDSS() { return m_pNoDepthWritesDSS; }
	ComPtr<ID3D11DepthStencilState> GetDisableDSS() { return m_pDisableDepthDSS; }
	ComPtr<ID3D11RasterizerState> GetRasterizerStateCCW() { return m_pRasterizerStateCCW; }
	ComPtr<ID3D11RasterizerState> GetRasterizerStateCW() { return m_pRasterizerStateCW; }

public:
	void Initalize(PipeLineInfo info);
	void Render();

private:
	void createSamplerState();
	void createBlendState();
	void createDepthStencilState();
	void createRasterizerState();

private:
	ComPtr<ID3D11Device> m_pDevice = nullptr;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr;

	ComPtr<ID3D11SamplerState> m_pSamplerLinear = nullptr;		// 텍스처 샘플러
	ComPtr<ID3D11SamplerState> m_pSamplerClamp = nullptr;		// 텍스처 샘플러
	ComPtr<ID3D11SamplerState> m_pShadowSampler = nullptr;		// 텍스처 샘플러
	ComPtr<ID3D11BlendState>   m_pAlphaBlendState = nullptr;		// 블렌드 상태 변경 (반투명 처리를 위한 블렌드상태)
	ComPtr<ID3D11BlendState>   m_pParticleBlendState = nullptr;		// 블렌드 상태 변경 (파티클 처리를 위한 블렌드상태)

	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCCW; // 컬 모드 CCW
	ComPtr<ID3D11RasterizerState> m_pRasterizerStateCW; // 컬 모드 CW
	ComPtr<ID3D11DepthStencilState> m_LessEqualDSS;
	ComPtr<ID3D11DepthStencilState> m_pNoDepthWritesDSS;			// 반투명 깊이 스텐실 스테이트 ( 깊이값 안쓰고 깊이값 사용 )
	ComPtr<ID3D11DepthStencilState> m_pDisableDepthDSS;				// 반투명 깊이 스텐실 스테이트 ( 깊이값 안쓰고 깊이값 안사용 )
};

