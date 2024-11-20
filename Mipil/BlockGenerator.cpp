#include "pch.h"
#include "BlockGenerator.h"

#include "YAMLParser.h"
#include "BlockObject.h"
#include "BlockPOD.h"

#include "PuzzleWorld.h"

BlockGenerator::BlockGenerator(std::shared_ptr<World> world, std::string_view filePath)
{
	m_ownerWorld = dynamic_cast<PuzzleWorld*>(world.get());

	std::shared_ptr<YAMLParser> parser = std::make_shared<YAMLParser>();
	parser->ReadBlockPOD(filePath, m_blockPODs);
}

BlockGenerator::~BlockGenerator()
{
}

void BlockGenerator::Generate()
{
	for(const auto& pod : m_blockPODs)
	{
		if (pod->blockType == eBlockType::END)
			continue;
		std::shared_ptr<BlockObject> block = m_ownerWorld->CreateGameObject<BlockObject>("block", eObjectType::LEVEL).lock();
		block->SetFilePath(MatchingFilePath(pod->blockType));
		block->SetBlockPOD(pod);
	}
}

std::string BlockGenerator::MatchingFilePath(eBlockType blockType)
{
	switch (blockType)
	{
	case eBlockType::STONE_BLUE_1:
		return "../Resources/FBX/1f_ingame_cube_blueemissive.fbx";
	case eBlockType::STONE_BLUE_2:
		return "../Resources/FBX/1f_ingame_cube2_blueemissive.fbx";
	case eBlockType::STONE_BLUE_3:
		return "../Resources/FBX/1f_ingame_cube3_blueemissive.fbx";
	case eBlockType::STONE_GREEN_1:
		return "../Resources/FBX/1f_ingame_cube_greenemissive.fbx";
	case eBlockType::STONE_GREEN_2:
		return "../Resources/FBX/1f_ingame_cube2_greenemissive.fbx";
	case eBlockType::STONE_GREEN_3:
		return "../Resources/FBX/1f_ingame_cube3_greenemissive.fbx";
	case eBlockType::STONE_YELLOW_1:
		return "../Resources/FBX/1f_ingame_cube_yellowemissive.fbx";
	case eBlockType::STONE_YELLOW_2:
		return "../Resources/FBX/1f_ingame_cube2_yellowemissive.fbx";
	case eBlockType::STONE_YELLOW_3:
		return "../Resources/FBX/1f_ingame_cube3_yellowemissive.fbx";
	case eBlockType::STONE_RED_1:
		return "../Resources/FBX/1f_ingame_cube_redemissive.fbx";
	case eBlockType::STONE_RED_2:
		return "../Resources/FBX/1f_ingame_cube2_redemissive.fbx";
	case eBlockType::STONE_RED_3:
		return "../Resources/FBX/1f_ingame_cube3_redemissive.fbx";
	//
	case eBlockType::STONE_BLUE_2F:
		return "../Resources/FBX/2f_ingame_cube_blueemissive.fbx";
	case eBlockType::STONE_GREEN_2F:
		return "../Resources/FBX/2f_ingame_cube_greenemissive.fbx";
	case eBlockType::STONE_YELLOW_2F:
		return "../Resources/FBX/2f_ingame_cube_yellowemissive.fbx";
	case eBlockType::STONE_RED_2F:
		return "../Resources/FBX/2f_ingame_cube_redemissive.fbx";
	//
	case eBlockType::TOTEM_BLUE_HEAD:
		return "../Resources/FBX/worm2_totem_head.fbx";
	case eBlockType::TOTEM_BLUE_BODY:
		return "../Resources/FBX/worm2_totem_body.fbx";
	case eBlockType::TOTEM_GREEN_HEAD:
		return "../Resources/FBX/bird_totem_head.fbx";
	case eBlockType::TOTEM_GREEN_BODY:
		return "../Resources/FBX/bird_totem_body.fbx";
	case eBlockType::TOTEM_YELLOW_HEAD:
		return "../Resources/FBX/bird_totem2_head.fbx";
	case eBlockType::TOTEM_YELLOW_BODY:
		return "../Resources/FBX/bird_totem2_body.fbx";
	case eBlockType::TOTEM_RED_HEAD:
		return "../Resources/FBX/worm_totem_head.fbx";
	case eBlockType::TOTEM_RED_BODY:
		return "../Resources/FBX/worm_totem_body.fbx";
	default:
		LOG_CONTENTS("정의되지 않은 블록 타입");
		assert(true);
		return "";
	}
}

