#pragma once
#include "Mesh.h"

class Animation;
class NodeAnimation;
class SkeletalMeshModel;
struct ModelResource;
struct aiNode;

namespace Math = DirectX::SimpleMath;

class Bone //: public Transform
{
public:
	std::string m_name;
	vector<Bone> m_Children;
	NodeAnimation* m_pCurNodeAnimation = nullptr;
	NodeAnimation* m_pNextNodeAnimation = nullptr;

	float m_AnimChangeTime = 0;
	float* m_pAnimationTime = nullptr;	// 현재 노드가 애니메이션에서 어느 시간에 있는지

	// Bone 행렬 계산(기존 Transform 기능 흡수)
	Math::Matrix m_localTransform;
	Math::Matrix m_worldTransform;
	Bone* m_pParentBone = nullptr;
	SkeletalMeshModel* m_pParentModel = nullptr;

public:
	void Update(float deltaTime);
	void SetParentBone(Bone* pParent) { m_pParentBone = pParent; };
	void SetParentMoel(SkeletalMeshModel* parentModel) { m_pParentModel = parentModel; }

	Bone* FindNode(const std::string& name);
	Bone& CreateChild();
};