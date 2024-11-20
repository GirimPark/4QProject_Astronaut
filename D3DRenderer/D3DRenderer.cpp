#include "pch.h"
#include "D3DRenderer.h"

#include <string>
#include <directxtk/SimpleMath.h>

D3DRenderer::D3DRenderer(int screenWidth, int screenHeight)
	: m_screenWidth(screenWidth)
	, m_screenHeight(screenHeight)
{
}

D3DRenderer::~D3DRenderer()
{
	CoUninitialize();
}

bool D3DRenderer::Initialize(HWND hwnd)
{
	// 결과값
	HRESULT hr = 0;

	/// 1. 스왑체인 속성 설정 구조체 생성
	// https://learn.microsoft.com/ko-kr/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	swapDesc.BufferCount = 2;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// Present()호출 후 디스플레이 화면에서 픽셀을 처리하는 옵션

	// BufferDesc : 백 버퍼 표시 모드
	// BufferDesc - 백버퍼(텍스처)의 가로/세로 크기 설정
	swapDesc.BufferDesc.Width = m_screenWidth;
	swapDesc.BufferDesc.Height = m_screenHeight;
	// BufferDesc - 화면 주사율 결정
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	// BufferDesc - 표시 형식
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// 다중 샘플링 매개 변수 설정
	swapDesc.SampleDesc.Count = 1;		// 다중 샘플링 하지 않음
	swapDesc.SampleDesc.Quality = 0;	// 다중 샘플링 하지 않음

	// 백 버퍼의 표면 사용량 및 CPU 액세스 옵션 설명
	// 해당 단계에서 백버퍼를 출력용으로 선언했기 때문에, RenderTargetView를 생성하는 부분에서 특정 플래그를 사용하지 않아도 된다.
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hwnd;
	swapDesc.Windowed = TRUE;


	/// 2. Device, DeviceContext, SwapChain 생성
	// https://learn.microsoft.com/ko-kr/windows/w11/nf-d3d11-d3d11createdeviceandswapchain
	UINT creationFlags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	HR_T(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, NULL, NULL,
		D3D11_SDK_VERSION, &swapDesc, m_pSwapChain.GetAddressOf(), m_pDevice.GetAddressOf(), NULL, m_pDeviceContext.GetAddressOf()));


	/// 3. RenderTargetView 생성, RTV에 백버퍼 바인딩
	// https://learn.microsoft.com/ko-kr/windows/win32/api/d3d11/nf-d3d11-id3d11device-createrendertargetview
	ID3D11Texture2D* pBackBufferTexture = nullptr;
	HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTexture));
	// pDesc == NULL일 경우, Mipmap을 사용하지 않는다.
	HR_T(m_pDevice->CreateRenderTargetView(pBackBufferTexture, NULL, m_pRenderTargetView.GetAddressOf()));	// 텍스처 내부 참조 증가
	SAFE_RELEASE(pBackBufferTexture);

	/// 4. 뷰포트 설정, Rasterization 단계에 바인딩
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(m_screenWidth);
	viewport.Height = static_cast<float>(m_screenHeight);
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;
	m_pDeviceContext->RSSetViewports(1, &viewport);


	/// 5. Depth&Stencil View 생성
	// depthDesc 설정으로 텍스쳐를 만들고, 해당 텍스쳐를 DSV의 표면 리소스로 사용하여 DSV 생성
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = m_screenWidth;
	depthDesc.Height = m_screenHeight;
	depthDesc.MipLevels = 1;	// Mipmap 사용하지 않음
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	ID3D11Texture2D* textureDepthStencil = nullptr;
	HR_T(m_pDevice->CreateTexture2D(&depthDesc, nullptr, &textureDepthStencil));

	D3D11_DEPTH_STENCIL_VIEW_DESC DSVDesc = {};
	DSVDesc.Format = depthDesc.Format;
	DSVDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	DSVDesc.Texture2D.MipSlice = 0;
	HR_T(m_pDevice->CreateDepthStencilView(textureDepthStencil, &DSVDesc, m_pDepthStencilView.GetAddressOf()));
	SAFE_RELEASE(textureDepthStencil);


	/// InitializeScene
	///	1. 버텍스 셰이더, 픽셀 셰이더 생성, 인풋 레이아웃 바인딩
	// 인풋 레이아웃은 버텍스 쉐이더가 입력받을 데이터의 형식을 지정한다.
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXTURE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"BLENDWEIGHTS", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	// vertex shader 1
	ID3DBlob* vertexShaderBuffer = nullptr; // 컴파일된 코드에 액세스할 포인터 변수
	const WCHAR* fileName = L"../Shader/StaticVertexShader.hlsl";
	m_vertexShaderMap.emplace(fileName, nullptr);
	m_pInputLayouts.emplace(fileName, nullptr);
	HR_T(CompileShaderFromFile(fileName, "main", "vs_5_0", &vertexShaderBuffer, nullptr));
	HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, m_vertexShaderMap[fileName].GetAddressOf()));
	HR_T(m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), m_pInputLayouts[fileName].GetAddressOf()));
	// vertex shader 2
	ZeroMemory(vertexShaderBuffer, sizeof(*vertexShaderBuffer));
	fileName = L"../Shader/SkeletalVertexShader.hlsl";
	m_vertexShaderMap.emplace(fileName, nullptr);
	HR_T(CompileShaderFromFile(fileName, "main", "vs_5_0", &vertexShaderBuffer, nullptr));
	HR_T(m_pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, m_vertexShaderMap[fileName].GetAddressOf()));
	HR_T(m_pDevice->CreateInputLayout(layout, ARRAYSIZE(layout),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), m_pInputLayouts[fileName].GetAddressOf()));
	SAFE_RELEASE(vertexShaderBuffer);

	// pixel shader
	ID3DBlob* pixelShaderBuffer = nullptr;
	fileName = L"../Shader/PBRPixelShader.hlsl";
	m_pixelShaderMap.emplace(fileName, nullptr);
	HR_T(CompileShaderFromFile(fileName, "main", "ps_5_0", &pixelShaderBuffer, nullptr));
	HR_T(m_pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, m_pixelShaderMap[fileName].GetAddressOf()));
	SAFE_RELEASE(pixelShaderBuffer);

	/// 2. 상수 버퍼 생성
	//	위치 상수 버퍼
	D3D11_BUFFER_DESC TCBDesc = {};
	ZeroMemory(&TCBDesc, sizeof(TCBDesc));
	TCBDesc.Usage = D3D11_USAGE_DEFAULT;
	TCBDesc.ByteWidth = sizeof(TransformCB);
	TCBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	TCBDesc.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&TCBDesc, nullptr, m_pTransformSubresource.GetAddressOf()));

	m_transformCB.mWorld = XMMatrixIdentity();
	XMVECTOR Eye = XMVectorSet(0.f, 0.f, -500.f, 0.f);
	XMVECTOR At = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	XMVECTOR Up = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	m_transformCB.mView = XMMatrixLookAtLH(Eye, At, Up);
	m_transformCB.mProjection = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.f), m_screenWidth / static_cast<FLOAT>(m_screenHeight), 0.3f, 1000.f);
	m_transformCB.mView = XMMatrixTranspose(m_transformCB.mView);
	m_transformCB.mProjection = XMMatrixTranspose(m_transformCB.mProjection);

	// 조명 상수 버퍼
	D3D11_BUFFER_DESC LCBDesc = {};
	ZeroMemory(&LCBDesc, sizeof(LCBDesc));
	LCBDesc.Usage = D3D11_USAGE_DEFAULT;
	LCBDesc.ByteWidth = sizeof(LightingCB);
	LCBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	LCBDesc.CPUAccessFlags = 0;
	HR_T(m_pDevice->CreateBuffer(&LCBDesc, nullptr, m_pLightingSubresource.GetAddressOf()));


	/// 3. sample state 생성
	D3D11_SAMPLER_DESC sampleDesc = {};
	ZeroMemory(&sampleDesc, sizeof(sampleDesc));
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampleDesc.MinLOD = 0;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	HR_T(m_pDevice->CreateSamplerState(&sampleDesc, m_pSamplerLinear.GetAddressOf()));

	/// 4. blend state 생성
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	D3D11_RENDER_TARGET_BLEND_DESC rtBlendDesc = {};
	rtBlendDesc.BlendEnable = true;
	// FinalRGB = SrcRGB * SrcBlend + DestRGB * DestBlend;
	rtBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	rtBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	// FinalAlpha = (SrcAlpha * SrcBlendAlpha) + (DestAlpha * DestBlendAlpha)
	rtBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	rtBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	rtBlendDesc.DestBlendAlpha = D3D11_BLEND_ONE;
	rtBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0] = rtBlendDesc;
	HR_T(m_pDevice->CreateBlendState(&blendDesc, m_pAlphaBlendState.GetAddressOf()));

	return true;
}

void D3DRenderer::Render()
{
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	DirectX::SimpleMath::Color color(.5f, .5f, .5f, 1.f);
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);

	// Draw 계열 함수를 호출하기 전 렌더링 파이프라인에 필수 스테이지 설정을 해야한다.
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	// 정점을 이어서 그리는 방식
	m_pDeviceContext->VSSetConstantBuffers(0, 1, m_pTransformSubresource.GetAddressOf());
	m_pDeviceContext->PSSetConstantBuffers(1, 1, m_pLightingSubresource.GetAddressOf());

	const WCHAR* vsFileName = L"../Shader/StaticVertexShader.hlsl";
	m_pDeviceContext->IASetInputLayout(m_pInputLayouts[vsFileName].Get());
	m_pDeviceContext->VSSetShader(m_vertexShaderMap[vsFileName].Get(), nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pixelShaderMap[L"../Shader/PBRPixelShader.hlsl"].Get(), nullptr, 0);

	m_pDeviceContext->PSSetSamplers(0, 1, m_pSamplerLinear.GetAddressOf());
	m_pDeviceContext->OMSetBlendState(m_pAlphaBlendState.Get(), nullptr, 0xFFFFFFFF);


	/// LightingConstantBuffer
	m_lightingCB.mLightDirection = Vector4{ 0.f, 0.f, 1.f, 1.f };
	m_lightingCB.mLightColor = Vector4{ 1.f, 1.f, 1.f, 1.f };
	m_lightingCB.mCameraTranslation = Vector3{ 0.f, 0.f, -500.f };
	m_lightingCB.mLightIntensity = 1.f;
	m_lightingCB.mAmbientPower = 94.f / 255.f;
	m_lightingCB.mSpecularPower = 2000.f;
	m_pDeviceContext->UpdateSubresource(m_pLightingSubresource.Get(), 0, nullptr, &m_lightingCB, 0, 0);

	/// TransformConstantBuffer
	m_transformCB.mWorld = XMMatrixTranspose(m_transformCB.mWorld);
	m_transformCB.mView = m_viewMatrix;
	//m_transformCB.mProjection = m_projectionMatrix;
	m_pDeviceContext->UpdateSubresource(m_pTransformSubresource.Get(), 0, nullptr, &m_transformCB, 0, 0);
}

void D3DRenderer::Finalize()
{
	for(auto& vertexShader : m_vertexShaderMap)
	{
		vertexShader.second.Reset();
	}
	for (auto& pixelShader : m_pixelShaderMap)
	{
		pixelShader.second.Reset();
	}

	m_vertexShaderMap.clear();
	m_pixelShaderMap.clear();
}
