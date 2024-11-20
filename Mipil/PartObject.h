#pragma once
#include "../Engine/GameObject.h"

struct PartPOD;
class StaticMeshComponent;

class PartObject
	: public GameObject
{
public:
	PartObject() = default;
	~PartObject() override = default;

	void SetFilePath(std::string_view filePath) { m_filePath = filePath; }
	void SetPartPOD(std::shared_ptr<PartPOD> pod) { m_partPOD = pod; }

	void Initialize() override;
	void Update(float deltaTime) override;

private:
	std::string m_filePath;
	std::shared_ptr<PartPOD> m_partPOD;
};
