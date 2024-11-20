#pragma once

enum class ePartType;
class World;
class PuzzleWorld;
struct PartPOD;

class PartGenerator
{
public:
	PartGenerator(std::shared_ptr<World> world, std::string_view filePath);
	~PartGenerator();

	void Generate();

private:
	std::string MatchingFilePath(ePartType partType);

private:
	std::vector<std::shared_ptr<PartPOD>> m_partPODs;
	PuzzleWorld* m_ownerWorld;
};

