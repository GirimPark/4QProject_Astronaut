#pragma once
#include "../Engine/GameObject.h"

struct BlockPOD;
class StaticMeshComponent;

class FloorBlockObject :
    public GameObject
{
public:
	FloorBlockObject() = default;
	~FloorBlockObject() override = default;

	void SetFilePath(std::string_view filePath) { m_filePath = filePath; }

	void Initialize() override;
	void Update(float deltaTime) override;

private:
	std::string m_filePath;
	std::shared_ptr<StaticMeshComponent> m_BlockMeshComponent;
};

