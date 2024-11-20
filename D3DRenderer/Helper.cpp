#include "pch.h"
#include "Helper.h"
#include <comdef.h>
#include <d3dcompiler.h>
 
#pragma comment(lib,"d3dcompiler.lib")
 
std::wstring ToWString(const std::string& s)
{
	std::wstring wsTmp(s.begin(), s.end());
	return wsTmp;
}

LPCWSTR GetComErrorString(HRESULT hr)
{
	_com_error err(hr);
	LPCWSTR errMsg = err.ErrorMessage();
	return errMsg;
}

void HR_T(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw com_exception(hr);
	}
} 

HRESULT CreateTextureFromFile(ID3D11Device* d3dDevice, const wchar_t* szFileName, ID3D11ShaderResourceView** textureView)
{
	HRESULT hr = S_OK;
	 
	// Load the Texture
	hr = DirectX::CreateDDSTextureFromFile(d3dDevice, szFileName, nullptr, textureView);
	if (FAILED(hr))
	{
		hr = DirectX::CreateWICTextureFromFile(d3dDevice, szFileName, nullptr, textureView);
		if (FAILED(hr))
		{
			MessageBoxW(NULL, GetComErrorString(hr), szFileName, MB_OK);
			return hr;
		}
	}
	return S_OK;
}
 
HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut, const D3D_SHADER_MACRO* pDefines)
{
	HRESULT hr = S_OK; 
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;

	hr = D3DCompileFromFile(szFileName, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			MessageBoxA(NULL, (char*)pErrorBlob->GetBufferPointer(), "CompileShaderFromFile", MB_OK);
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

float RandF()
{
	return float(rand()) / (float)RAND_MAX;
}

float RandF(float a, float b)
{
	return a + RandF() * (b - a);
}

// 랜덤 1D 텍스트 생성
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateRandomTexture1DSRV(ID3D11Device* device)
{
	using namespace DirectX::SimpleMath;

	// 랜덤 벡터 생성
	const size_t ELEMENT_COUNT = 1024u;

	std::vector<Vector4> randomValues(ELEMENT_COUNT);

	for (size_t i = 0; i < ELEMENT_COUNT; ++i)
	{
		randomValues[i].x = RandF(-1.f, 1.f);
		randomValues[i].y = RandF(-1.f, 1.f);
		randomValues[i].z = RandF(-1.f, 1.f);
		randomValues[i].w = RandF(-1.f, 1.f);
	}

	// 텍스쳐 생성
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = ELEMENT_COUNT;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &randomValues[0];
	initData.SysMemPitch = ELEMENT_COUNT * sizeof(Vector4);
	initData.SysMemSlicePitch = 0;

	ID3D11Texture1D* randomTex = 0;
	HR_T(device->CreateTexture1D(&texDesc, &initData, &randomTex));

	// 쉐이더 리소스 뷰 생성
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip = 0;

	ComPtr<ID3D11ShaderResourceView> randomTexSRV = nullptr;
	HR_T(device->CreateShaderResourceView(randomTex, &viewDesc, randomTexSRV.GetAddressOf()));

	SAFE_RELEASE(randomTex);

	return randomTexSRV;
}

