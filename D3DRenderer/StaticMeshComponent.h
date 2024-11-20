#pragma once
#include <vector>
#include <list>
#include <memory>
#include <string>
#include <wrl.h>

#include "StaticMeshInstance.h"
#include "Transform.h"

class Material;
class ModelResource;

class StaticMeshComponent : public Transform
{
public:
	StaticMeshComponent();
	~StaticMeshComponent();

	std::string m_SceneFilePath; // BeginPlay에서 로딩
	std::shared_ptr<ModelResource>  m_ModelResource;
	std::vector<StaticMeshInstance> m_MeshInstances;

	std::list<StaticMeshComponent*>::iterator m_iterator;

	bool ReadSceneResourceFromFBX(std::string filePath);
	void SetSceneResource(std::shared_ptr<ModelResource> val);

	Material* GetMaterial(UINT index);

	virtual void Update(float DeltaTime) override;
	virtual void OnBeginPlay();
	virtual void OnEndPlay();
};
