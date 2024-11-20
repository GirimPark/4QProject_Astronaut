#include "pch.h"
#include "ParticleSystem.h"

#include "Helper.h"
#include "D3DRenderManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "PipeLine.h"

ParticleSystem::ParticleSystem()
	: m_FirstRun(true)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Init(wstring TexturePath, UINT maxParticles, eParticleType particleType, wstring shaderPath)
{
	m_TexturePath = TexturePath;
	m_ParticleType = particleType;
	m_MaxParticles = maxParticles;
	m_ShaderPath = shaderPath;
	m_FirstRun = true;
	m_RandomTexture = CreateRandomTexture1DSRV(D3DRenderManager::m_pDevice.Get());

	// 상수 버퍼 생성
	ParticleVertex p;
	ZeroMemory(&p, sizeof(ParticleVertex));
	p.Age = 0.0f;
	p.Type = 0;

	// 스트림 출력용 정점 버퍼 만들기
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(ParticleVertex) * 1;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &p;
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&vbd, &vinitData, &m_InitVertexBuffer)); // 초기화용 방출기 입자

	vbd.ByteWidth = sizeof(ParticleVertex) * m_MaxParticles; // 최대로 다룰 버퍼 크기, 넘어가면 안댐
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT; // 플래그 중요!
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&vbd, 0, &m_DrawVertexBuffer));
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&vbd, 0, &m_StreamOutVertexBuffer));
}

void ParticleSystem::Reset()
{
	m_FirstRun = true;
	m_Age = 0.0f;
}

void ParticleSystem::Update(float delayTime, float gameTime)
{
	m_CBPerFrame.GameTime = gameTime;
	m_CBPerFrame.TimeStep = delayTime;
	m_Age += delayTime;
}

void ParticleSystem::Render(ComPtr<ID3D11DeviceContext> deviceContext, weak_ptr<PipeLine> pipeLine)
{
	m_Stride = sizeof(ParticleVertex);
	m_Offset = 0;

	textureSetting(deviceContext);
	streamOutPipeLineSetting(deviceContext, pipeLine);

	// 렌더
	// 처음 그릴 때는 버퍼 하나만 그린다. 아니면 정점 갯수만큼 알아서 draw 해준다.
	if (m_FirstRun) { deviceContext->Draw(1, 0); m_FirstRun = false; }
	else			{ deviceContext->DrawAuto(); }

	drawPipeLineSetting(deviceContext, pipeLine);
	deviceContext->DrawAuto();
	endPipeLineSetting(deviceContext, pipeLine);
}

void ParticleSystem::textureSetting(ComPtr<ID3D11DeviceContext> deviceContext)
{
	// 파티클 셰이더 및 텍스쳐 불러오기
	auto texture = ResourceManager::GetInstance()->CreateTextureResource(m_TexturePath.c_str());

	// 리소스 및 파이프라인 세팅
	deviceContext->VSSetShaderResources(20, 1, texture->GetTexture().GetAddressOf());
	deviceContext->VSSetShaderResources(21, 1, m_RandomTexture.GetAddressOf());
	deviceContext->GSSetShaderResources(20, 1, texture->GetTexture().GetAddressOf());
	deviceContext->GSSetShaderResources(21, 1, m_RandomTexture.GetAddressOf());
	deviceContext->PSSetShaderResources(20, 1, texture->GetTexture().GetAddressOf());
	deviceContext->PSSetShaderResources(21, 1, m_RandomTexture.GetAddressOf());
}

void ParticleSystem::streamOutPipeLineSetting(ComPtr<ID3D11DeviceContext> deviceContext, weak_ptr<PipeLine> pipeLine)
{
	auto shader = ResourceManager::GetInstance()->CreateParticleShader(m_ShaderPath, m_ParticleType);

	// 인덱스 버퍼 세팅 해제
	// 토폴로지 변경 ( 포인트 )
	// 깊이 스텐실 스테이트 세팅 ( 깊이 쓰기 끄기, 깊이 버퍼 사용 끄기 )
	deviceContext->IASetIndexBuffer(nullptr, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	deviceContext->OMSetDepthStencilState(pipeLine.lock()->GetDisableDSS().Get(), 0);	// 깊이 쓰기 끄기

	// 스트림 아웃으로 정점 생성/삭제
	shader->StreamOutShaderSetting(deviceContext.Get());
	deviceContext->SOSetTargets(1					// 묶을 정점 갯수, 최대 4개
		, m_StreamOutVertexBuffer.GetAddressOf()	// 출력용 버퍼
		, &m_Offset);									// 정점 버퍼마다 기록하기 시작할 위치를 나타냄, 현재 0

	// 바인딩
	// 처음 그릴 땐 초기화 버퍼로 그린다. 아니면 그냥 그리기용 버퍼 사용.
	if (m_FirstRun)	
	{
		deviceContext->IASetVertexBuffers(0, 1, m_InitVertexBuffer.GetAddressOf(), &m_Stride, &m_Offset);
	}
	else			
	{
		deviceContext->IASetVertexBuffers(0, 1, m_DrawVertexBuffer.GetAddressOf(), &m_Stride, &m_Offset);
	}
}

void ParticleSystem::drawPipeLineSetting(ComPtr<ID3D11DeviceContext> deviceContext, weak_ptr<PipeLine> pipeLine)
{
	auto shader = ResourceManager::GetInstance()->CreateParticleShader(m_ShaderPath, m_ParticleType);
	ID3D11Buffer* bufferArray[1] = { NULL };
	float factor[] = { 0.f, 0.f, 0.f, 0.f };

	// 드로우 파이프라인 세팅
	// 블렌드 스테이트 ( 가산 혼합 )
	// 깊이 스텐실 스테이트 ( 깊이 버퍼 사용, 쓰지 않음 )
	// 버텍스 버퍼 세팅 ( 드로우 버텍스 세팅 )
	shader->DrawShaderSetting(deviceContext.Get());
	deviceContext->OMSetBlendState(pipeLine.lock()->GetParticleBlendState().Get(), factor, 0xffffffff);
	deviceContext->OMSetDepthStencilState(pipeLine.lock()->GetNoDepthWritesDSS().Get(), 0);
	deviceContext->IASetVertexBuffers(0, 1, m_DrawVertexBuffer.GetAddressOf(), &m_Stride, &m_Offset);

	// 
	deviceContext->SOSetTargets(1, bufferArray, &m_Offset);	// 출력 단계를 비워준다.
	swap(m_DrawVertexBuffer, m_StreamOutVertexBuffer);		// 핑퐁을 위해 swap 한다.
}

void ParticleSystem::endPipeLineSetting(ComPtr<ID3D11DeviceContext> deviceContext, weak_ptr<PipeLine> pipeLine)
{
	auto shader = ResourceManager::GetInstance()->CreateParticleShader(m_ShaderPath, m_ParticleType);
	float factor[] = { 0.f, 0.f, 0.f, 0.f };

	// 초기화 세팅 하기
	// 셰이더 초기화
	// 블렌드 스테이트 초기화
	// 깊이 스텐실 스테이트 모두 쓰기, 사용으로 변경
	shader->NullShaderSetting(deviceContext.Get());
	deviceContext->OMSetBlendState(0, factor, 0xffffffff);
	deviceContext->OMSetDepthStencilState(pipeLine.lock()->GetLessEqualDSS().Get(), 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ParticleSystem::UpdatePerFrame(CB_PerFrame& perFrame)
{
	perFrame.GameTime = m_CBPerFrame.GameTime;
	perFrame.TimeStep = m_CBPerFrame.TimeStep;
	perFrame.EmitGenerationRange = m_CBPerFrame.EmitGenerationRange;
	perFrame.EmitPosWorld = m_CBPerFrame.EmitPosWorld;
	perFrame.EyePosWorld = m_CBPerFrame.EyePosWorld;
}

void ParticleSystem::UpdatePerticleData(CB_ParticleData& particleData)
{
	particleData.Acceleration = m_CBParticleData.Acceleration;
	particleData.CreatePerSecond = m_CBParticleData.CreatePerSecond;
	particleData.InitialSpeed = m_CBParticleData.InitialSpeed;
	particleData.LifeTime = m_CBParticleData.LifeTime;
	particleData.ParticleDirection = m_CBParticleData.ParticleDirection;
	particleData.PlayParticle = m_CBParticleData.PlayParticle;
	particleData.Size = m_CBParticleData.Size;
}
