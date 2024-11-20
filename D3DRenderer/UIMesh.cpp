#include "pch.h"
#include "UIMesh.h"

#include "D3DRenderManager.h"

UIMesh::UIMesh()
{
}

UIMesh::~UIMesh()
{
}

void UIMesh::Create(Math::Vector2 size)
{
	m_UIVertices.resize(4);
	float halfX = (size.x / 2.f);
	float halfY = (size.y / 2.f);

	m_UIVertices[0].Position = Vector4(-halfX, -halfY, 0.f, 1.f);
	m_UIVertices[0].TexCoord = Vector2(0.f, 1.f);
	m_UIVertices[1].Position = Vector4(-halfX, halfY, 0.f, 1.f);
	m_UIVertices[1].TexCoord = Vector2(0.f, 0.f);
	m_UIVertices[2].Position = Vector4(halfX, -halfY, 0.f, 1.f);
	m_UIVertices[2].TexCoord = Vector2(1.f, 1.f);
	m_UIVertices[3].Position = Vector4(halfX, halfY, 0.f, 1.f);
	m_UIVertices[3].TexCoord = Vector2(1.f, 0.f);
	m_Indices = { 0,1,2,2,1,3 };

	CreateVertexBuffer();
	CreateIndexBuffer();
}

void UIMesh::CreateIndexBuffer()
{
	/// CreateIndexBuffer  ///
	// 인덱스 개수 저장.
	m_IndexCount = m_Indices.size();

	D3D11_BUFFER_DESC indexBD = {};
	indexBD.ByteWidth = sizeof(UINT) * m_IndexCount;
	indexBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBD.Usage = D3D11_USAGE_DEFAULT;
	indexBD.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA ibData = {};
	ibData.pSysMem = m_Indices.data();
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&indexBD, &ibData, m_pIndexBuffer.GetAddressOf()));
}

void UIMesh::CreateVertexBuffer()
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(UIVertex) * m_UIVertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = m_UIVertices.data();
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &vbData, m_pVertexBuffer.GetAddressOf()));

	// 버텍스 버퍼 정보
	m_VertexCount = m_UIVertices.size();
	m_VertexBufferStride = sizeof(UIVertex);
	m_VertexBufferOffset = 0;
}