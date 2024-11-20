#pragma once

#include <d3d11.h>
#include <directxtk/SimpleMath.h>
#include <directxtk/BufferHelpers.h>
#include <wrl/client.h>
#include <unordered_map>

using namespace Microsoft::WRL;
using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// Renderer를 싱글톤으로 만들지 않기위해 싱글톤인 CommonApp(Core)를 활용한다.
/// GameApp 단에서는 렌더러의 존재를 몰라야 하고,
/// Engine까지만 렌더러의 존재를 안다.
/// </summary>

struct TransformCB
{
	Matrix mWorld;	//TODO
	Matrix mView;
	Matrix mProjection;
};

struct LightingCB
{
	Vector4 mLightDirection;
	Vector4 mLightColor;
	Vector3 mCameraTranslation;
	FLOAT mLightIntensity;
	FLOAT mAmbientPower;
	FLOAT mSpecularPower;
	FLOAT garbage1;
	FLOAT garbage2;
};

class D3DRenderer
{
private:
	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;
	ComPtr<IDXGISwapChain> m_pSwapChain;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;

	//TODO : meshInstance에 원하는 vertex, pixel Shader 정보를 넣어서 셰이더 선택
	std::unordered_map<const WCHAR*, ComPtr<ID3D11VertexShader>> m_vertexShaderMap;
	std::unordered_map<const WCHAR*, ComPtr<ID3D11PixelShader>> m_pixelShaderMap;

	std::unordered_map<const WCHAR*, ComPtr<ID3D11InputLayout>> m_pInputLayouts;			// 입력 레이아웃
	ComPtr<ID3D11SamplerState> m_pSamplerLinear;		// 샘플러 상태
	ComPtr<ID3D11BlendState> m_pAlphaBlendState;		// 블렌더 상태

	ComPtr<ID3D11Buffer> m_pTransformSubresource;
	ComPtr<ID3D11Buffer> m_pLightingSubresource;
	TransformCB m_transformCB;
	LightingCB m_lightingCB;

	const int m_screenWidth;
	const int m_screenHeight;

	Matrix m_viewMatrix;
	Matrix m_projectionMatrix;

public:
	ComPtr<ID3D11Device> GetDevice() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDevcon() { return m_pDeviceContext; }
	ComPtr<IDXGISwapChain> GetSwapChain() { return m_pSwapChain; }

	void SetTransformCB(Matrix transform) { m_transformCB.mWorld = transform; }
	void SetViewMatrix(const Matrix& viewMatrix) { m_viewMatrix = viewMatrix; }
	void SetProjectionMatrix(const Matrix& projectionMatrix) { m_projectionMatrix = projectionMatrix; }

public:
	D3DRenderer(int screenWidth, int screenHeight);
	~D3DRenderer();

public:
	bool Initialize(HWND hwnd);
	void Render();
	void Finalize();
};

