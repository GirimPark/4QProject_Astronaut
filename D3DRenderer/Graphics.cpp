#include "pch.h"
#include "Graphics.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

void Graphics::Initalize(GraphicsInfo info)
{
	m_hWnd = info.m_hwnd;
	m_ClientWidth = info.m_ClientWidth;
	m_ClientHeight = info.m_ClientHeight;
	m_ClearColor = info.m_ClearColor;

	setViewPort();
	createDirect2D();
	createSwapChain();
	createRenderTargetView();
	createStencilAndDepth();
}


/// 그림자 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 ///
void Graphics::RenderBeginShadow()
{
	// 현재 쉐이더 리소스 세팅을 언바인딩
	ID3D11ShaderResourceView* pSRV = nullptr;
	m_pDeviceContext->PSSetShaderResources(11, 1, &pSRV);
	SAFE_RELEASE(pSRV);

	// 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 및 클리어
	m_pDeviceContext->RSSetViewports(1, &m_ShadowViewport);
	m_pDeviceContext->OMSetRenderTargets(0, NULL, m_pShadowDepthStencilView.Get());
	m_pDeviceContext->ClearDepthStencilView(m_pShadowDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	m_pDeviceContext->PSSetShader(NULL, NULL, 0);
}


/// 외곽선 두께 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 ///
void Graphics::RenderBeginOutLineOver()
{
	// 현재 쉐이더 리소스 세팅을 언바인딩
	ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;
	m_pDeviceContext->PSSetShaderResources(15, 1, pSRV.GetAddressOf());
	m_pDeviceContext->PSSetShaderResources(16, 1, pSRV.GetAddressOf());

	// 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 및 클리어
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);
	m_pDeviceContext->OMSetRenderTargets(0, NULL, m_pOutLineOverDepthStencilView.Get());
	m_pDeviceContext->ClearDepthStencilView(m_pOutLineOverDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	m_pDeviceContext->PSSetShader(NULL, NULL, 0);
}


/// 외곽선 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 ///
void Graphics::RenderBeginOutLine()
{
	// 현재 쉐이더 리소스 세팅을 언바인딩
	ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;
	m_pDeviceContext->PSSetShaderResources(15, 1, pSRV.GetAddressOf());
	m_pDeviceContext->PSSetShaderResources(16, 1, pSRV.GetAddressOf());

	// 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 및 클리어
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);
	m_pDeviceContext->OMSetRenderTargets(0, NULL, m_pOutLineDepthStencilView.Get());
	m_pDeviceContext->ClearDepthStencilView(m_pOutLineDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
	m_pDeviceContext->PSSetShader(NULL, NULL, 0);
}


/// 3D UI Mesh 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 ///
void Graphics::RenderBeginUIMesh_1()
{
	// 현재 쉐이더 리소스 세팅을 언바인딩
	ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;
	m_pDeviceContext->PSSetShaderResources(18, 1, pSRV.GetAddressOf());
	m_pDeviceContext->PSSetShaderResources(19, 1, pSRV.GetAddressOf());

	// 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 및 클리어
	float color[4] = { 0.f, 0.f, 0.f, 0.f };
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);
	m_pDeviceContext->OMSetRenderTargets(1, m_pUIMesh_1_RenderTargetView.GetAddressOf(), m_pUIMeshDepthStencilView.Get());
	m_pDeviceContext->ClearRenderTargetView(m_pUIMesh_1_RenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pUIMeshDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->PSSetShader(NULL, NULL, 0);
}


/// 3D UI Mesh 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 ///
void Graphics::RenderBeginUIMesh_2()
{
	// 현재 쉐이더 리소스 세팅을 언바인딩
	ComPtr<ID3D11ShaderResourceView> pSRV = nullptr;
	m_pDeviceContext->PSSetShaderResources(18, 1, pSRV.GetAddressOf());
	m_pDeviceContext->PSSetShaderResources(19, 1, pSRV.GetAddressOf());

	// 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 및 클리어
	float color[4] = { 0.f, 0.f, 0.f, 0.f };
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);
	m_pDeviceContext->OMSetRenderTargets(1, m_pUIMesh_2_RenderTargetView.GetAddressOf(), m_pUIMeshDepthStencilView.Get());
	m_pDeviceContext->ClearRenderTargetView(m_pUIMesh_2_RenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pUIMeshDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	m_pDeviceContext->PSSetShader(NULL, NULL, 0);
}


/// 3D 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 ///
void Graphics::RenderBegin()
{
	// 현재 쉐이더 리소스 세팅을 언바인딩
	ID3D11ShaderResourceView* pSRV = nullptr;
	m_pDeviceContext->PSSetShaderResources(17, 1, &pSRV);
	SAFE_RELEASE(pSRV);

	// 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 및 클리어
	float color[4] = { m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w };
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);
	m_pDeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}


/// 최종 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 ///
void Graphics::RenderBeginPostProcessing()
{
	// 렌더 타겟 뷰 및 뎁스 스텐실 뷰 세팅 및 클리어
	float color[4] = { m_ClearColor.x, m_ClearColor.y, m_ClearColor.z, m_ClearColor.w };
	m_pDeviceContext->RSSetViewports(1, &m_Viewport);
	m_pDeviceContext->OMSetRenderTargets(1, m_pFinalRenderTargetView.GetAddressOf(), m_pFinalDepthStencilView.Get());
	m_pDeviceContext->ClearRenderTargetView(m_pFinalRenderTargetView.Get(), color);
	m_pDeviceContext->ClearDepthStencilView(m_pFinalDepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	// 각 텍스쳐 세팅
	m_pDeviceContext->PSSetShaderResources(15, 1, m_pOutLineResourceView.GetAddressOf());
	m_pDeviceContext->PSSetShaderResources(16, 1, m_pOutLineOverResourceView.GetAddressOf());
	m_pDeviceContext->PSSetShaderResources(17, 1, m_pRenderResourceView.GetAddressOf());
	m_pDeviceContext->PSSetShaderResources(18, 1, m_pUIMesh_1_RenderResourceView.GetAddressOf());
	m_pDeviceContext->PSSetShaderResources(19, 1, m_pUIMesh_2_RenderResourceView.GetAddressOf());

	//d2d
	GetD2DRenderTarget()->BeginDraw();
}

void Graphics::RenderEnd()
{
	//d2d
	GetD2DRenderTarget()->EndDraw();

	HRESULT hr = m_pSwapChain->Present(0, 0);
	assert(SUCCEEDED(hr));
}

void Graphics::createSwapChain()
{
	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = m_hWnd;	// 스왑체인 출력할 창 핸들 값.
	swapDesc.Windowed = true;		// 창 모드 여부 설정.
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 백버퍼(텍스처)의 가로/세로 크기 설정.
	swapDesc.BufferDesc.Width = m_ClientWidth;
	swapDesc.BufferDesc.Height = m_ClientHeight;
	// 화면 주사율 설정.

	swapDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapDesc.BufferDesc.RefreshRate.Denominator = 0;
	// 샘플링 관련 설정.
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;

	swapDesc.BufferCount = 2;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	// Present()호출 후 디스플레이 화면에서 픽셀을 처리하는 옵션
	swapDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT creationFlags = D3D10_CREATE_DEVICE_BGRA_SUPPORT; //D2D 때문에 BGRA 서포트 해줘야함
#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// 1. 장치 생성.   2.스왑체인 생성. 3.장치 컨텍스트 생성.
	HR_T(D3D11CreateDeviceAndSwapChain
	(
		  NULL
		, D3D_DRIVER_TYPE_HARDWARE
		, NULL
		, creationFlags
		, NULL
		, NULL
		, D3D11_SDK_VERSION
		, &swapDesc
		, &m_pSwapChain
		, &m_pDevice
		, NULL
		, &m_pDeviceContext)
	);
}

void Graphics::createDirect2D()
{
	HR_T(DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(m_pdWriteFactory.GetAddressOf())
	));

	HR_T(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, m_pd2dFactory.GetAddressOf()));
}

void Graphics::createRenderTargetView()
{
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = m_ClientWidth;
	descDepth.Height = m_ClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	// D3D Render Target
	{
		// 원근 투영에서 3D Model들을 띄울 렌더 타겟 뷰
		ComPtr<ID3D11Texture2D> pBackBufferTexture = nullptr;
		HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, &pBackBufferTexture));
		HR_T(m_pDevice->CreateRenderTargetView(pBackBufferTexture.Get(), NULL, m_pRenderTargetView.GetAddressOf()));  // 텍스처는 내부 참조 증가
		HR_T(m_pDevice->CreateShaderResourceView(pBackBufferTexture.Get(), &srvDesc, m_pRenderResourceView.GetAddressOf()));

		// 3D Mesh를 직교 투영에서 띄우고 텍스쳐처럼 사용할 렌더 타겟 뷰
		ComPtr<ID3D11Texture2D> pUIMesh_1_BufferTexture = nullptr;
		HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, &pUIMesh_1_BufferTexture));
		HR_T(m_pDevice->CreateRenderTargetView(pUIMesh_1_BufferTexture.Get(), NULL, m_pUIMesh_1_RenderTargetView.GetAddressOf()));  // 텍스처는 내부 참조 증가
		HR_T(m_pDevice->CreateShaderResourceView(pUIMesh_1_BufferTexture.Get(), &srvDesc, m_pUIMesh_1_RenderResourceView.GetAddressOf()));

		// 3D Mesh를 직교 투영에서 띄우고 텍스쳐처럼 사용할 렌더 타겟 뷰
		ComPtr<ID3D11Texture2D> pUIMesh_2_BufferTexture = nullptr;
		HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, &pUIMesh_2_BufferTexture));
		HR_T(m_pDevice->CreateRenderTargetView(pUIMesh_2_BufferTexture.Get(), NULL, m_pUIMesh_2_RenderTargetView.GetAddressOf()));  // 텍스처는 내부 참조 증가
		HR_T(m_pDevice->CreateShaderResourceView(pUIMesh_2_BufferTexture.Get(), &srvDesc, m_pUIMesh_2_RenderResourceView.GetAddressOf()));

		// 최종 렌더 타겟 뷰
		ComPtr<ID3D11Texture2D> pfinalBufferTexture = nullptr;
		HR_T(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pfinalBufferTexture));
		HR_T(m_pDevice->CreateRenderTargetView(pfinalBufferTexture.Get(), NULL, m_pFinalRenderTargetView.GetAddressOf()));  // 텍스처는 내부 참조 증가
		HR_T(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(m_pdxgiBackbuffer.GetAddressOf())));
	}

	// D2D Render Target
	{
		D2D1_RENDER_TARGET_PROPERTIES props =
			D2D1::RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				0,
				0);

		HR_T(m_pd2dFactory->CreateDxgiSurfaceRenderTarget(
			m_pdxgiBackbuffer.Get(),
			props,
			m_pd2dRenderTargetView.GetAddressOf()));
	}
}

void Graphics::createStencilAndDepth()
{
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = m_ClientWidth;
	descDepth.Height = m_ClientHeight;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	ID3D11Texture2D* textureDepthStencil = nullptr;
	HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, &textureDepthStencil));

	HR_T(m_pDevice->CreateDepthStencilView(textureDepthStencil, &descDSV, m_pDepthStencilView.GetAddressOf()));
	SAFE_RELEASE(textureDepthStencil);

	ID3D11Texture2D* uiMeshDepthStencil = nullptr;
	HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, &uiMeshDepthStencil));

	HR_T(m_pDevice->CreateDepthStencilView(uiMeshDepthStencil, &descDSV, m_pUIMeshDepthStencilView.GetAddressOf()));
	SAFE_RELEASE(uiMeshDepthStencil);

	ID3D11Texture2D* finalTextureDepthStencil = nullptr;
	HR_T(m_pDevice->CreateTexture2D(&descDepth, nullptr, &finalTextureDepthStencil));

	HR_T(m_pDevice->CreateDepthStencilView(finalTextureDepthStencil, &descDSV, m_pFinalDepthStencilView.GetAddressOf()));
	SAFE_RELEASE(finalTextureDepthStencil);

	// 그림자용 ShadowMap Texture와 SRV, DSV 생성
	{
		// Create Shadow map texture desc
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = static_cast<float>(SHADOWMAP_SIZE);
		texDesc.Height = static_cast<float>(SHADOWMAP_SIZE);
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;

		ID3D11Texture2D* pShadowMap = nullptr;
		HR_T(m_pDevice->CreateTexture2D(&texDesc, NULL, &pShadowMap));

		// Create the depth stencil view desc
		D3D11_DEPTH_STENCIL_VIEW_DESC ShadowdescDSV = {};
		ShadowdescDSV.Format = DXGI_FORMAT_D32_FLOAT;
		ShadowdescDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		HR_T(m_pDevice->CreateDepthStencilView(pShadowMap, &ShadowdescDSV, m_pShadowDepthStencilView.GetAddressOf()));

		// Create the depth stencil view desc
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		HR_T(m_pDevice->CreateShaderResourceView(pShadowMap, &srvDesc, m_pShadowResourceView.GetAddressOf()));
		SAFE_RELEASE(pShadowMap);
	}
	// 외곽선용 OverLine Texture와 SRV, DSV 생성
	{
		// Create Shadow map texture desc
		D3D11_TEXTURE2D_DESC texDesc = {};
		texDesc.Width = m_ClientWidth;
		texDesc.Height = m_ClientHeight;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;

		ID3D11Texture2D* pOutLineOverMap = nullptr;
		ID3D11Texture2D* pOutLineMap = nullptr;
		HR_T(m_pDevice->CreateTexture2D(&texDesc, NULL, &pOutLineOverMap));
		HR_T(m_pDevice->CreateTexture2D(&texDesc, NULL, &pOutLineMap));

		// Create the depth stencil view desc
		D3D11_DEPTH_STENCIL_VIEW_DESC OutLineDSV = {};
		OutLineDSV.Format = DXGI_FORMAT_D32_FLOAT;
		OutLineDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		

		// Create the Shader Resource View Desc
		D3D11_SHADER_RESOURCE_VIEW_DESC OutLineSRV = {};
		OutLineSRV.Format = DXGI_FORMAT_R32_FLOAT;
		OutLineSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		OutLineSRV.Texture2D.MipLevels = 1;

		HR_T(m_pDevice->CreateDepthStencilView(pOutLineOverMap, &OutLineDSV, m_pOutLineOverDepthStencilView.GetAddressOf()));
		HR_T(m_pDevice->CreateShaderResourceView(pOutLineOverMap, &OutLineSRV, m_pOutLineOverResourceView.GetAddressOf()));
		HR_T(m_pDevice->CreateDepthStencilView(pOutLineMap, &OutLineDSV, m_pOutLineDepthStencilView.GetAddressOf()));
		HR_T(m_pDevice->CreateShaderResourceView(pOutLineMap, &OutLineSRV, m_pOutLineResourceView.GetAddressOf()));

		SAFE_RELEASE(pOutLineOverMap);
		SAFE_RELEASE(pOutLineMap);
	}
}

void Graphics::setViewPort()
{
	m_Viewport.TopLeftX = 0.f;
	m_Viewport.TopLeftY = 0.f;
	m_Viewport.Width = static_cast<float>(m_ClientWidth);
	m_Viewport.Height = static_cast<float>(m_ClientHeight);
	m_Viewport.MinDepth = 0.f;
	m_Viewport.MaxDepth = 1.f;

	m_ShadowViewport.TopLeftX = 0.f;
	m_ShadowViewport.TopLeftY = 0.f;
	m_ShadowViewport.Width = static_cast<float>(SHADOWMAP_SIZE);
	m_ShadowViewport.Height = static_cast<float>(SHADOWMAP_SIZE);
	m_ShadowViewport.MinDepth = 0.f;
	m_ShadowViewport.MaxDepth = 1.f;
}
