#include "pch.h"
#include "StaticMeshInstance.h"
#include "Mesh.h"

StaticMeshInstance::StaticMeshInstance()
{
}

StaticMeshInstance::~StaticMeshInstance()
{

}


void StaticMeshInstance::Create(Mesh* pMeshResource, Math::Matrix* pWorld, Material* pMaterial, BoundingBox* pboundingBox)
{
	m_pMeshResource = pMeshResource;
	m_pMaterial = pMaterial;
	m_pNodeWorldTransform = pWorld;
	m_pBoundingBox = pboundingBox;
}

void StaticMeshInstance::Render(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetIndexBuffer(m_pMeshResource->m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetVertexBuffers(0, 1, m_pMeshResource->m_pVertexBuffer.GetAddressOf(), &m_pMeshResource->m_VertexBufferStride, &m_pMeshResource->m_VertexBufferOffset);
	deviceContext->DrawIndexed(m_pMeshResource->m_IndexCount, 0, 0);
}
