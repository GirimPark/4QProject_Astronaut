#include "pch.h"
#include "PartGenerator.h"

#include "YAMLParser.h"
#include "PartObject.h"
#include "PartPOD.h"

#include "PuzzleWorld.h"

PartGenerator::PartGenerator(std::shared_ptr<World> world, std::string_view filePath)
{
	m_ownerWorld = dynamic_cast<PuzzleWorld*>(world.get());

	std::shared_ptr<YAMLParser> parser = std::make_shared<YAMLParser>();
	parser->ReadPartPOD(filePath, m_partPODs);
}

PartGenerator::~PartGenerator()
{
}

void PartGenerator::Generate()
{
	for (const auto& pod : m_partPODs)
	{
		if ((pod->partType == ePartType::END))
			continue;
		std::shared_ptr<PartObject> block = m_ownerWorld->CreateGameObject<PartObject>("part", eObjectType::PART).lock();
		block->SetFilePath(MatchingFilePath(pod->partType));
		block->SetPartPOD(pod);

		m_ownerWorld->AddNumParts();
	}
}

std::string PartGenerator::MatchingFilePath(ePartType partType)
{
	switch (partType)
	{
	case ePartType::GEAR:
		return "../Resources/FBX/gear_fix.fbx";
	default:
		LOG_CONTENTS("정의되지 않은 블록 타입");
		assert(true);
		return "";
	}
}
