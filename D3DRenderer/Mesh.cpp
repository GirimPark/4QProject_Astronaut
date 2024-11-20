#include "pch.h"
#include "D3DRenderManager.h"
#include "Helper.h"
#include "Mesh.h"
#include "SkeletonResource.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::Create(aiMesh* mesh)
{
	m_MaterialIndex = mesh->mMaterialIndex;

	// 버텍스 정보 생성
	m_Vertices.resize(mesh->mNumVertices);

	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		m_Vertices[i].Position = Vector4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
		m_Vertices[i].TexCoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		m_Vertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		m_Vertices[i].Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
	}

	CreateVertexBuffer<Vertex>(&m_Vertices[0], (UINT)m_Vertices.size(), m_pVertexBuffer.GetAddressOf());

	// 인덱스 정보 생성
	m_Indices.resize(mesh->mNumFaces * 3);

	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		m_Indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		m_Indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		m_Indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}

	CreateIndexBuffer();
}

void Mesh::CreateBoneWeight(aiMesh* mesh, SkeletonResource* skeleton)
{
	m_MaterialIndex = mesh->mMaterialIndex;
	m_MeshName = mesh->mName.C_Str();

	// 버텍스 정보 생성
	m_BoneWeightVertices.resize(mesh->mNumVertices);

	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		m_BoneWeightVertices[i].Position = Vector4(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 1.0f);
		m_BoneWeightVertices[i].TexCoord = Vector2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		m_BoneWeightVertices[i].Normal = Vector3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		m_BoneWeightVertices[i].Tangent = Vector3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
	}

	UINT meshBoneCount = mesh->mNumBones;     // 메쉬와 연결된 본 개수
	m_BoneReferences.resize(meshBoneCount);   // 본 연결 정보 컨테이너 크기 조절

	// 메쉬와 연결된 본들을 처리
	UINT boneIndexCounter = 0;
	std::map<std::string, int> BoneMapping;

	for (UINT i = 0; i < meshBoneCount; ++i)
	{
		aiBone* pAiBone = mesh->mBones[i];

		// 스켈레톤에서 본정보를 찾는다.
		UINT boneIndex = skeleton->GetBoneIndexByBoneName(pAiBone->mName.C_Str());
		assert(boneIndex != -1);
		BoneInfo* pBone = skeleton->GetBone(boneIndex);
		assert(pBone != nullptr);

		m_BoneReferences[i].NodeName = pAiBone->mName.C_Str();
		m_BoneReferences[i].BoneIndex = boneIndex;
		m_BoneReferences[i].OffsetMatrix = Math::Matrix(&pAiBone->mOffsetMatrix.a1).Transpose();

		// 본과 연결된 버텍스들을 처리
		for (UINT j = 0; j < pAiBone->mNumWeights; ++j)
		{
			UINT vertexID = pAiBone->mWeights[j].mVertexId;
			float weight = pAiBone->mWeights[j].mWeight;
			m_BoneWeightVertices[vertexID].AddBoneData(boneIndex, weight);
		}
	}
	CreateVertexBuffer<BoneWeightVertex>(&m_BoneWeightVertices[0], (UINT)m_BoneWeightVertices.size(), m_pBWVertexBuffer.GetAddressOf());

	//=======================================================//
	// 인덱스 정보 생성
	m_Indices.resize(mesh->mNumFaces * 3);

	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		m_Indices[i * 3 + 0] = mesh->mFaces[i].mIndices[0];
		m_Indices[i * 3 + 1] = mesh->mFaces[i].mIndices[1];
		m_Indices[i * 3 + 2] = mesh->mFaces[i].mIndices[2];
	}

	CreateIndexBuffer();
}

void Mesh::CreateIndexBuffer()
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