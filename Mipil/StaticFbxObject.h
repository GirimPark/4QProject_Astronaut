#pragma once
#include "../Engine/GameObject.h"

class StaticMeshComponent;

class StaticFbxObject
	: public GameObject
{
public:
	void Initialize() override;
	void Update(float deltaTime) override;

	void SetFilePath(std::string filePath);
	void SetDeferredPosition(Vector3 pos) { m_deferredPosition = pos; }

	std::weak_ptr<StaticMeshComponent> GetWeakMeshComponent() {	return m_meshComponent;	}


private:
	std::weak_ptr<StaticMeshComponent> m_meshComponent;
	std::string m_filePath;

	Vector3 m_deferredPosition;
};

