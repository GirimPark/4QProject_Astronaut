#pragma once
#include <d3d11.h>
#include <string>
#include <vector>
#include <directxtk\SimpleMath.h>
#include <wrl/client.h>
using namespace DirectX::SimpleMath;

class Model;
class NodeAnimation;
class Mesh;

class Node
{
private:
	std::string m_name;
	Model* m_pOwner;
	Node* m_pParentNode;
	std::vector<Node*> m_pChildren;
	Matrix m_localTransform;
	Matrix m_worldTransform;
	NodeAnimation* m_pNodeAnimation = nullptr;
	int m_curIdx = 0;
	float m_elapsedTime;
	float m_animationSpeed = 1.f;
	std::vector<Mesh*> m_meshes;

	bool m_bBoneLinked = false;

	ComPtr<ID3D11Buffer> m_pNodeTransformConstantBuffer;

public:
	void AddChildNode(Node* child);

public:
	Node(std::string name, Model* pOwner, Matrix localTransform, const std::vector<Mesh*>& meshes);
	~Node();

public:
	void SetAnimation(NodeAnimation* pNodeAnimation) { this->m_pNodeAnimation = pNodeAnimation; }
	std::string GetName() { return m_name; }
	std::vector<Node*>& GetChildren() { return m_pChildren; }
	DirectX::XMMATRIX GetWorldTransform() { return m_localTransform; }
	void SetWorldTransform(DirectX::XMMATRIX transform) { m_localTransform = transform; m_bBoneLinked = true; }
	void LinkWorldTransform(Matrix*& pTransform) { pTransform = &m_worldTransform; }
	void SetAnimationSpeed(float speed) { m_animationSpeed = speed; }
	bool GetBoneLinked() { return m_bBoneLinked; }

public:
	void Update(float deltaTime);
	void Render();

private:
	void SetupNode();
	void UpdateTransform();
};

