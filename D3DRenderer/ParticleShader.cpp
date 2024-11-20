#include "pch.h"
#include "ParticleShader.h"

#include "ParticleSystem.h"
#include "Helper.h"
#include "D3DRenderManager.h"


ParticleShader::ParticleShader()
{

}

ParticleShader::~ParticleShader()
{

}

void ParticleShader::Create(const wstring& shaderFileName, eParticleType type)
{
	m_Name = shaderFileName;
	m_Type = type;

	const D3D11_INPUT_ELEMENT_DESC particleDesc[5] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"VELOCITY", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"SIZE",     0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"AGE",      0, DXGI_FORMAT_R32_FLOAT,       0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TYPE",     0, DXGI_FORMAT_R32_UINT,        0, 36, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	ID3DBlob* blob = nullptr;
	HR_T(CompileShaderFromFile(shaderFileName.c_str(), "StreamOutVS", "vs_5_0", &blob, nullptr));
	HR_T(D3DRenderManager::m_pDevice->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		NULL,
		&m_StreamOutVertexShader));
	SAFE_RELEASE(blob);

	HR_T(CompileShaderFromFile(shaderFileName.c_str(), "DrawVS", "vs_5_0", &blob, nullptr));
	HR_T(D3DRenderManager::m_pDevice->CreateVertexShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		NULL,
		&m_DrawVertexShader));
	HR_T(D3DRenderManager::m_pDevice->CreateInputLayout(
		particleDesc,
		ARRAYSIZE(particleDesc),
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		&m_InputLayout
	)); 
	SAFE_RELEASE(blob);

	// 스트림 출력용 기하 셰이더
	D3D11_SO_DECLARATION_ENTRY desc[] =
	{
		{ 0, "POSITION", 0, 0, 3, 0},
		{ 0, "VELOCITY", 0, 0, 3, 0},
		{ 0, "SIZE", 0, 0, 2, 0},
		{ 0, "AGE", 0, 0, 1, 0},
		{ 0, "TYPE", 0, 0, 1, 0},
	};
	UINT bufferStrides[] = { sizeof(ParticleVertex) };
	HR_T(CompileShaderFromFile(shaderFileName.c_str(), "StreamOutGS", "gs_5_0", &blob, nullptr));
	HR_T(D3DRenderManager::m_pDevice->CreateGeometryShaderWithStreamOutput(
		blob->GetBufferPointer(), // 컴파일된 진입점
		blob->GetBufferSize(), // 컴파일된 사이즈
		desc, // 정점 형식
		ARRAYSIZE(desc), // 정점 형식 개수
		bufferStrides, // 정점 사이즈
		ARRAYSIZE(bufferStrides),
		0, // 래스터라이저 단계로 보낼 스트림 인덱스 번호
		NULL,
		&m_StreamOutGeometryShader)); // out 매개변수
	SAFE_RELEASE(blob);

	HR_T(CompileShaderFromFile(shaderFileName.c_str(), "DrawGS", "gs_5_0", &blob, nullptr));
	HR_T(D3DRenderManager::m_pDevice->CreateGeometryShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		NULL,
		&m_DrawGeometryShader));
	SAFE_RELEASE(blob);

	HR_T(CompileShaderFromFile(shaderFileName.c_str(), "DrawPS", "ps_5_0", &blob, nullptr));
	HR_T(D3DRenderManager::m_pDevice->CreatePixelShader(
		blob->GetBufferPointer(),
		blob->GetBufferSize(),
		NULL,
		&m_DrawPixelShader));
	SAFE_RELEASE(blob);
}

void ParticleShader::StreamOutShaderSetting(ID3D11DeviceContext* deviceContext)
{
	// 스트림 아웃으로 정점 생성/삭제
	deviceContext->IASetInputLayout(m_InputLayout.Get());
	deviceContext->VSSetShader(m_StreamOutVertexShader.Get(), nullptr, 0);
	deviceContext->GSSetShader(m_StreamOutGeometryShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(NULL, nullptr, 0); // 픽셀 셰이더 바인딩 안하기
}

void ParticleShader::DrawShaderSetting(ID3D11DeviceContext* deviceContext)
{
	// 화면 렌더링
	deviceContext->VSSetShader(m_DrawVertexShader.Get(), nullptr, 0);
	deviceContext->GSSetShader(m_DrawGeometryShader.Get(), nullptr, 0);
	deviceContext->PSSetShader(m_DrawPixelShader.Get(), nullptr, 0);
}

void ParticleShader::NullShaderSetting(ID3D11DeviceContext* deviceContext)
{
	deviceContext->VSSetShader(NULL, nullptr, 0);
	deviceContext->GSSetShader(NULL, nullptr, 0);
	deviceContext->PSSetShader(NULL, nullptr, 0);
}
