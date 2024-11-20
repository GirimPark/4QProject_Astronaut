#pragma once
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <vector>
#include <map>
#include <memory>
#include <wrl/client.h>

#include "Animation.h"
#include "Mesh.h"

struct Vertex;
struct FrameKey;
struct NodeAnimation;
struct Animation;
class Node;
class Mesh;


struct Bone
{
	std::string Name;
	Matrix offsetMatrix;	// 본 기준 mesh의 offsetMatrix
	Matrix* pBoneMatrix = nullptr;	// Node에 있는 WorldTransform 참조
};

struct BoneMatrixConstantBuffer
{
	Matrix Array[128];
};

class Model
{
private:
	Node* m_pRootNode;
	std::vector<Animation*> m_pAnimations;
	int m_curAnimationIdx = 0;
	bool m_bAnimation;
	float m_tickPerSecond;

	std::string m_directory;
	std::string m_filePath;

	std::vector<Texture> m_loadedTextures;

	bool m_bBone;
	std::vector<Bone> m_bones;
	std::map<std::string, int> m_boneMap;
	int m_boneIndex = 0;
	int m_indexCount = 0;

	ComPtr<ID3D11Buffer> m_pBoneMatrixConstantBuffer;

public:
	Model(std::string filepath);
	~Model();

	void Load();
	void Update(float deltaTime);
	void Render();

public:
	DirectX::XMMATRIX GetTransform();
	void SetTransform(DirectX::XMMATRIX transform);
	void SetAnimationSpeed(float speed) { m_pAnimations[m_curAnimationIdx]->AnimationSpeed = speed; }
	float GetAnimationSpeed() { return m_pAnimations[m_curAnimationIdx]->AnimationSpeed; }
	float GetAnimationDuration() { return m_pAnimations[m_curAnimationIdx]->AnimationDuration; }
	bool GetbAnimation() { return m_bAnimation; }

private:
	void ParsingNode(aiNode* pNode, Node* pParentNode, const aiScene* pScene);
	Mesh* ParsingMesh(aiMesh* mesh, const aiScene* pScene);
	void ProcessBoneInfo(aiMesh* mesh, std::vector<Vertex>& vertices);
	bool ParsingAnimation(const aiScene* pScene);
	std::vector<NodeAnimation*> ParsingNodeAnimation(aiAnimation* pAnimation);
	std::vector<FrameKey> ParsingFrameKey(aiNodeAnim* pNodeAnim);
	void AssignAnimation(Node* node);

	void AssignBone(Node* node);
	void UpdateBoneMatrix();

	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName, const aiScene* scene);
	ID3D11ShaderResourceView* LoadEmbeddedTexture(const aiTexture* embeddedTexture);
	bool SaveEmbeddedTexture(const aiTexture* embeddedTexture, std::string fileName);
};


