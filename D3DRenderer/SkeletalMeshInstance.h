#pragma once

class Mesh;
class Material;
class Bone;
struct CB_MatrixPalette;

class SkeletalMeshInstance
{
public:
	SkeletalMeshInstance();
	~SkeletalMeshInstance();

public:
	Mesh* m_pMeshResource = nullptr;
	Material* m_pMaterial = nullptr;
	Math::Matrix* m_pNodeWorldTransform = nullptr;		// SkeletalMesh의 월드행렬을 가진 노드의 포인터
	std::vector<Math::Matrix*>		m_BoneReferences;	// 본에 해당되는 노드MATRIX의 포인터를 저장한다.

public:
	void Create(Mesh* pMeshResource, Bone* pRootNode, Material* pMaterial);
	void BindingMaterial(Material* pMaterial);
	void UpdateMatrixPallete(Math::Matrix* MatrixPalettePtr);
	virtual void Render(ID3D11DeviceContext* deviceContext);
};

