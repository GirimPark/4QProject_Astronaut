#pragma once

#include "SkeletonResource.h"
#include "Material.h"
#include "Mesh.h"

class Animation;

enum class ModelType
{
	STATIC,
	SKELETAL
};

// 그냥 mesh 랑 material , anim  만들어서 갖고 있는 애임.
class ModelResource
{
public:
	ModelResource();
	~ModelResource();

	Vector3 m_AABBmin;
	Vector3 m_AABBmax;
	Vector3 m_BoundingBoxMin;
	Vector3 m_BoundingBoxMax;
	BoundingBox m_BoundingBox;
	vector<BoundingBox> m_InstanceBoundingBox;

	vector<Mesh> m_Meshes;
	vector<Material> m_Materials;
	SkeletonResource m_Skeleton;

	std::vector<std::shared_ptr<Animation>> m_Animations;

public:
	bool ReadFile(std::string filePath, ModelType modelType);
	Material* GetMeshMaterial(UINT index);

	Math::Vector3 GetBoundingBoxDimensions() const;
};

