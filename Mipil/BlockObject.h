#pragma once
#include "../Engine/GameObject.h"

struct BlockPOD;
class StaticMeshComponent;

class BlockObject
	: public GameObject
{
public:
	BlockObject() = default;
	~BlockObject() override = default;

	void SetFilePath(std::string_view filePath) { m_filePath = filePath; }
	void SetBlockPOD(std::shared_ptr<BlockPOD> pod) { m_blockPOD = pod; }

	void Initialize() override;
	void Update(float deltaTime) override;

private:
	std::string m_filePath;
	std::shared_ptr<BlockPOD> m_blockPOD;
};