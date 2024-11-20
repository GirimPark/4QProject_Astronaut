#pragma once

#include "StaticMeshInstance.h"

class Material;
class ModelResource;

class StaticMeshModel
{
public:
	StaticMeshModel();
	~StaticMeshModel();

	Material* GetMaterial(UINT index);
	const std::vector<Mesh>& GetMeshes();

	void SetWorldMatrix(Math::Matrix* worldMatrix) { m_worldMatrix = worldMatrix; }
	void SetOutLine(bool isOutLine) { m_IsOutLine = isOutLine; }

	bool ReadSceneResourceFromFBX(std::string filePath);

	Math::Vector3 GetModelDimensions() const;

private:
	void SetSceneResource(std::shared_ptr<ModelResource> val);


public:
	bool m_IsCulled = true;
	bool m_IsOutLine = false;

	std::string m_SceneFilePath; // BeginPlay에서 로딩
	std::shared_ptr<ModelResource>  m_ModelResource;
	std::vector<StaticMeshInstance> m_MeshInstances;

	Math::Matrix* m_worldMatrix = nullptr;	// meshComponent의 matrix 참조
	BoundingBox m_BoundingBox;
};
