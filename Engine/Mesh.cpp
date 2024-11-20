#include "pch.h"
#include "Mesh.h"

using namespace DirectX;

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<UINT>& indices,
	const std::vector<Texture>& textures, SimpleMath::Vector4 baseColor)
		: m_vertices(vertices)
	, m_indices(indices)
	, m_textures(textures)
	, m_baseColor(baseColor)
{
	this->SetupMesh();
}

Mesh::~Mesh()
{
	m_vertices.clear();
	m_indices.clear();

	m_textures.clear();
}

void Mesh::Render()
{
	TextureMapConstantBuffer TextureMapCB;

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	CommonApp::m_pInstance->GetDevcon()->IASetVertexBuffers(0, 1, m_pVertexBuffer.GetAddressOf(), &stride, &offset);
	CommonApp::m_pInstance->GetDevcon()->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	CommonApp::m_pInstance->GetDevcon()->PSSetConstantBuffers(2, 1, m_pTextureMapConstantBuffer.GetAddressOf());

	for (const auto& texture : m_textures)
	{
		if (texture.Type == "texture_diffuse")
		{
			CommonApp::m_pInstance->GetDevcon()->PSSetShaderResources(0, 1, texture.Source.GetAddressOf());
			TextureMapCB.UseDiffuse = true;
		}
		if (texture.Type == "texture_normal")
		{
			CommonApp::m_pInstance->GetDevcon()->PSSetShaderResources(1, 1, texture.Source.GetAddressOf());
			TextureMapCB.UseNormal = true;
		}
		if (texture.Type == "texture_specular")
		{
			CommonApp::m_pInstance->GetDevcon()->PSSetShaderResources(2, 1, texture.Source.GetAddressOf());
			TextureMapCB.UseSpecular = true;
		}
		if (texture.Type == "texture_emissive")
		{
			CommonApp::m_pInstance->GetDevcon()->PSSetShaderResources(3, 1, texture.Source.GetAddressOf());
			TextureMapCB.UseEmissive = true;
		}
		if (texture.Type == "texture_opacity")
		{
			CommonApp::m_pInstance->GetDevcon()->PSSetShaderResources(4, 1, texture.Source.GetAddressOf());
			TextureMapCB.UseOpacity = true;
		}
		if (texture.Type == "texture_metalness")
		{
			CommonApp::m_pInstance->GetDevcon()->PSSetShaderResources(5, 1, texture.Source.GetAddressOf());
			TextureMapCB.UseMetalness = true;
		}
		if (texture.Type == "texture_roughness")
		{
			CommonApp::m_pInstance->GetDevcon()->PSSetShaderResources(6, 1, texture.Source.GetAddressOf());
			TextureMapCB.UseRoughness = true;
		}
	}

	if (TextureMapCB.UseDiffuse == false)
	{
		TextureMapCB.BaseColor = m_baseColor;
	}

	CommonApp::m_pInstance->GetDevcon()->UpdateSubresource(m_pTextureMapConstantBuffer.Get(), 0, nullptr, &TextureMapCB, 0, 0);

	CommonApp::m_pInstance->GetDevcon()->DrawIndexed(static_cast<UINT>(m_indices.size()), 0, 0);
}

void Mesh::SetupMesh()
{
	D3D11_BUFFER_DESC vertexBD;
	vertexBD.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBD.ByteWidth = static_cast<UINT>(sizeof(Vertex) * m_vertices.size());
	vertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBD.CPUAccessFlags = 0;
	vertexBD.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &m_vertices[0];
	HR_T(CommonApp::m_pInstance->GetDevice()->CreateBuffer(&vertexBD, &initData, m_pVertexBuffer.GetAddressOf()));

	D3D11_BUFFER_DESC indexBD;
	indexBD.Usage = D3D11_USAGE_IMMUTABLE;
	indexBD.ByteWidth = static_cast<UINT>(sizeof(UINT) * m_indices.size());
	indexBD.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBD.CPUAccessFlags = 0;
	indexBD.MiscFlags = 0;

	initData.pSysMem = &m_indices[0];
	HR_T(CommonApp::m_pInstance->GetDevice()->CreateBuffer(&indexBD, &initData, m_pIndexBuffer.GetAddressOf()));


	D3D11_BUFFER_DESC TCBDesc = {};
	ZeroMemory(&TCBDesc, sizeof(TCBDesc));
	TCBDesc.Usage = D3D11_USAGE_DEFAULT;
	TCBDesc.ByteWidth = sizeof(TextureMapConstantBuffer);
	TCBDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	TCBDesc.CPUAccessFlags = 0;
	HR_T(CommonApp::m_pInstance->GetDevice()->CreateBuffer(&TCBDesc, nullptr, m_pTextureMapConstantBuffer.GetAddressOf()));
}