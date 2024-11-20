#pragma once

#include "D3DRenderManager.h"

using namespace std;

class SkeletonResource;

struct Vertex
{
	Vector4 Position;    // 정점 위치 정보
	Vector2 TexCoord;
	Vector3 Normal;
	Vector3 Tangent;
};

// Skinning 전용 vertex 이고 다른 모델을 띄울 때는 그냥 vertex를 사용.
struct BoneWeightVertex
{
	Vector4 Position;    // 정점 위치 정보
	Vector2 TexCoord;
	Vector3 Normal;
	Vector3 Tangent;

	// 참조하는 본의 인덱스의 범위는 최대 128개로 일단 처리하자. 
	// 버텍스가 참조하는 본이 최대 4개로 제한했는데 그 본에 대한 Index
	int BlendIndeces[4] = {};  // 영향받는 본 수는 최대 4개로 제한한다.

	// 버텍스들이 참조하는 본의 weight 값.
	// 최대 4개를 참조하니까 weight 값도 4개까지 있게 됨.
	float BlendWeights[4] = {};  // 가중치 총합은 1이 되어야 한다.

	void AddBoneData(int boneIndex, float weight)
	{
		// 적어도 하나는 데이터가 비어있어야 한다.
		// 4개 이상의 값이 들어갈까봐 넣어준 예외 코드.
		assert(BlendWeights[0] == 0.0f || BlendWeights[1] == 0.0f ||
			BlendWeights[2] == 0.0f || BlendWeights[3] == 0.0f);

		for (int i = 0; i < 4; i++)
		{
			if (BlendWeights[i] == 0.0f)
			{
				BlendIndeces[i] = boneIndex;
				BlendWeights[i] = weight;
				return;
			}
		}
	}
};

struct BoneReference
{
	std::string NodeName; // 노드 이름
	Matrix OffsetMatrix;  // 본기준 메시의 OffsetMatrix
	Matrix* NodeWorldMatrixPtr;
	int BoneIndex = -1;			 // 본 인덱스
}; 

struct aiMesh;

class Mesh
{
public:
	Mesh();
	~Mesh();

public:
	void Create(aiMesh* mesh);
	void CreateBoneWeight(aiMesh* mesh, SkeletonResource* skeleton);
	void CreateIndexBuffer();

	template<typename T>
	void CreateVertexBuffer(T* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer);

 //	void UpdateMatrixPalette(Matrix* MatrixPalettePrt);

	std::vector<BoneWeightVertex>	m_BoneWeightVertices;
	std::vector<Vertex>				m_Vertices;
	std::vector<UINT>				m_Indices;

	ComPtr<ID3D11Buffer> m_pBWVertexBuffer;
	ComPtr<ID3D11Buffer> m_pVertexBuffer;
	ComPtr<ID3D11Buffer> m_pIndexBuffer;

	vector<BoneReference> m_BoneReferences;
	Matrix* m_pNodeWorld;

	UINT m_VertexCount = 0;
	UINT m_VertexBufferStride = 0;		// 버텍스 하나의 크기.
	UINT m_VertexBufferOffset = 0;		// 버텍스 버퍼의 오프셋.
	UINT m_IndexCount = 0;				// 인덱스 개수.
	UINT m_MaterialIndex = 0;			// 메테리얼 인덱스.

	string m_MeshName;
};

template<typename T>
inline void Mesh::CreateVertexBuffer(T* vertices, UINT vertexCount, ID3D11Buffer** vertexBuffer)
{
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(T) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA vbData = {};
	vbData.pSysMem = vertices;
	HR_T(D3DRenderManager::m_pDevice->CreateBuffer(&bd, &vbData, vertexBuffer));

	// 버텍스 버퍼 정보
	m_VertexCount = vertexCount;
	m_VertexBufferStride = sizeof(T);
	m_VertexBufferOffset = 0;
}
