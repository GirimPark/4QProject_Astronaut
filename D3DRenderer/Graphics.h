#pragma once

// ±◊∏≤¿⁄ ∏  «ÿªÛµµ
constexpr int SHADOWMAP_SIZE = 8192;

struct GraphicsInfo
{
	HWND m_hwnd;
	UINT m_ClientWidth = 800;
	UINT m_ClientHeight = 600;
	DirectX::SimpleMath::Vector4 m_ClearColor = DirectX::SimpleMath::Vector4(0.5f, 0.5f, 0.5f, 0.5f);
};

class Graphics
{
public:
	Graphics();
	~Graphics();

public:
	ComPtr<ID3D11Device> GetDevice() { return m_pDevice; }
	ComPtr<ID3D11DeviceContext> GetDeviceContext() { return m_pDeviceContext; }
	ComPtr<ID2D1RenderTarget> GetD2DRenderTarget() { return m_pd2dRenderTargetView; }
	ComPtr<IDXGISwapChain> GetSwapChain() { return m_pSwapChain; }
	ComPtr<IDWriteFactory> GetWriteFactory() { return m_pdWriteFactory; }
	D3D11_VIEWPORT GetViewport() { return m_Viewport; }
	ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() { return m_pShadowResourceView; }
	ComPtr<ID3D11ShaderResourceView> GetOutLineOverResourceView() { return m_pOutLineOverResourceView; }
	ComPtr<ID3D11ShaderResourceView> GetOutLineResourceView() { return m_pOutLineResourceView; }
	ComPtr<ID3D11ShaderResourceView> GetRenderResourceView() { return m_pRenderResourceView; }
	ComPtr<ID3D11ShaderResourceView> GetUIMesh_1_RenderResourceView() { return m_pUIMesh_1_RenderResourceView; }
	ComPtr<ID3D11ShaderResourceView> GetUIMesh_2_RenderResourceView() { return m_pUIMesh_2_RenderResourceView; }

public:
	void Initalize(GraphicsInfo info);

	void RenderBeginShadow();
	void RenderBeginOutLineOver();
	void RenderBeginOutLine();
	void RenderBeginUIMesh_1();
	void RenderBeginUIMesh_2();
	void RenderBegin();
	void RenderBeginPostProcessing();
	void RenderEnd();

private:
	void createSwapChain();
	void createDirect2D();
	void createRenderTargetView();
	void createStencilAndDepth();
	void setViewPort();

private:
	HWND m_hWnd;
	float m_ClientWidth;
	float m_ClientHeight;
	DirectX::SimpleMath::Vector4 m_ClearColor;

	ComPtr<ID3D11Device> m_pDevice;
	ComPtr<ID3D11DeviceContext> m_pDeviceContext = nullptr; // ¡ÔΩ√ µπŸ¿ÃΩ∫ ƒ¡≈ÿΩ∫∆Æ
	ComPtr<IDXGISwapChain> m_pSwapChain = nullptr;					// Ω∫ø“√º¿Œ

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;			// ∑ª¥ı∏µ ≈∏∞Ÿ∫‰
	ComPtr<ID3D11ShaderResourceView> m_pRenderResourceView;					// ∑ª¥ı∏µ Ω¶¿Ã¥ı ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11RenderTargetView> m_pUIMesh_1_RenderTargetView = nullptr;	// ∑ª¥ı∏µ ≈∏∞Ÿ∫‰
	ComPtr<ID3D11ShaderResourceView> m_pUIMesh_1_RenderResourceView;		// ∑ª¥ı∏µ Ω¶¿Ã¥ı ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11RenderTargetView> m_pUIMesh_2_RenderTargetView = nullptr;	// ∑ª¥ı∏µ ≈∏∞Ÿ∫‰
	ComPtr<ID3D11ShaderResourceView> m_pUIMesh_2_RenderResourceView;		// ∑ª¥ı∏µ Ω¶¿Ã¥ı ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;						// ∑ª¥ı∏µ ±Ì¿Ã Ω∫≈ŸΩ« ∫‰
	ComPtr<ID3D11DepthStencilView> m_pUIMeshDepthStencilView;				// UIMesh ∑ª¥ı∏µ ±Ì¿Ã Ω∫≈ŸΩ« ∫‰
	ComPtr<ID3D11DepthStencilView> m_pFinalDepthStencilView;				// √÷¡æ ∑ª¥ı∏µ ±Ì¿Ã Ω∫≈ŸΩ« ∫‰
	ComPtr<ID3D11RenderTargetView> m_pFinalRenderTargetView = nullptr;		// ∑ª¥ı∏µ ≈∏∞Ÿ∫‰

	ComPtr<ID3D11DepthStencilView> m_pShadowDepthStencilView;			// ±◊∏≤¿⁄ ±Ì¿Ã Ω∫≈ŸΩ« ∫‰
	ComPtr<ID3D11ShaderResourceView> m_pShadowResourceView;				// ±◊∏≤¿⁄ Ω¶¿Ã¥ı ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11DepthStencilView> m_pOutLineOverDepthStencilView;		// ø‹∞˚º± ø¿πˆ ±Ì¿Ã Ω∫≈ŸΩ« ∫‰
	ComPtr<ID3D11ShaderResourceView> m_pOutLineOverResourceView;		// ø‹∞˚º± ø¿πˆ Ω¶¿Ã¥ı ∏Æº“Ω∫ ∫‰
	ComPtr<ID3D11DepthStencilView> m_pOutLineDepthStencilView;			// ø‹∞˚º± ±Ì¿Ã ∫Ò±≥∏¶ ¿ß«— ±‚¡∏ ∏µ® ±Ì¿Ã Ω∫≈ŸΩ« ∫‰
	ComPtr<ID3D11ShaderResourceView> m_pOutLineResourceView;			// ø‹∞˚º± ±Ì¿Ã ∫Ò±≥∏¶ ¿ß«— ±‚¡∏ ∏µ® Ω¶¿Ã¥ı ∏Æº“Ω∫ ∫‰

	ComPtr<ID2D1Factory> m_pd2dFactory;								//Direct2D
	ComPtr<ID2D1Device> m_pd2dDevice;
	ComPtr<ID2D1DeviceContext> m_pd2dDeviceContext;
	ComPtr<ID2D1RenderTarget> m_pd2dRenderTargetView;
	ComPtr<IDXGISurface> m_pdxgiBackbuffer;
	ComPtr<IDWriteFactory> m_pdWriteFactory;						//DirectWrite

	D3D11_VIEWPORT m_Viewport = {};
	D3D11_VIEWPORT m_ShadowViewport = {};							// ±◊∏≤¿⁄ ∫‰∆˜∆Æ
};

