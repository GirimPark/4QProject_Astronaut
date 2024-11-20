#pragma once

struct BoneInfo
{
	Math::Matrix RelativeTransform;			//부모로부터의 상대적인 변환
	Math::Matrix OffsetMatrix;				// 본에서의 메쉬의 상대적인 위치(변환)
	string Name;						// 본이름
	int NumChildren = 0;						// 자식의 수
	int ParentBoneIndex = -1;
	vector<string> MeshNames;		// 본에 연결된 메쉬들의 이름
	BoneInfo() {}
	void Set(const aiNode* pNode)
	{
		Name = string(pNode->mName.C_Str());
		RelativeTransform = Math::Matrix(&pNode->mTransformation.a1).Transpose();
		NumChildren = pNode->mNumChildren;
	}
	~BoneInfo()
	{

	}
};

// 본 정보를 저장하는 구조체
struct SkeletonResource
{
public:
	string Name;
	vector<BoneInfo> Bones;
	map<string, int> BoneMappingTable;
	map<string, int> MeshMappingTable;

	void Create(const aiScene* pScene);
	BoneInfo* AddBone(const aiScene* pScene, const aiNode* pNode);
	BoneInfo* FindBone(const string& name);
	BoneInfo* GetBone(int index);
	int GetBoneIndexByBoneName(const string& boneName);
	int GetBoneIndexByMeshName(const string& meshName);
	int GetBoneCount() {
		return (int)Bones.size();
	}
	const string& GetBoneName(int index) {
		return Bones[index].Name;
	}
	void CountNode(int& Count, const aiNode* pNode);
};
