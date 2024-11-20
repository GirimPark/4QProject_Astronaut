#include "pch.h"
#include "YAMLParser.h"

#include "BlockPOD.h"
#include "PartPOD.h"

// yaml-cpp
#define YAML_CPP_STATIC_DEFINE
#include <yaml-cpp/yaml.h>

#ifdef _DEBUG
#pragma comment(lib, "yaml-cppd.lib")
#else
#pragma comment(lib, "yaml-cpp.lib")
#endif

void YAMLParser::ReadBlockPOD(std::string_view filePath, std::vector<std::shared_ptr<BlockPOD>>& blockPODs)
{
	try
	{
		std::vector<YAML::Node> blockNodes = YAML::LoadAllFromFile(std::string(filePath));
		

		for(const auto& node : blockNodes)
		{
			if (!node["PrefabInstance"])
				continue;

			std::shared_ptr<BlockPOD> block = std::make_shared<BlockPOD>();
			for(const auto& modification : node["PrefabInstance"]["m_Modification"]["m_Modifications"])
			{
				std::string property = modification["propertyPath"].as<std::string>();
				// Type
				if (property == "m_Name")
				{
					block->blockType = GetBlockType(modification["value"].as<std::string>());

					if (block->blockType == eBlockType::END)
                        continue;
				}

				// Position
				if (property == "m_LocalPosition.x")
				{
					block->position.x = modification["value"].as<float>() * 100;
				}
                if (property == "m_LocalPosition.y")
                {
                    block->position.y = modification["value"].as<float>() * 100 + BASE_HEIGHT;
                }
                if (property == "m_LocalPosition.z")
                {
                    block->position.z = modification["value"].as<float>() * 100;
                }

                // Rotation
                if (property == "m_LocalRotation.x")
                {
                    block->rotation.x = modification["value"].as<float>();
                }
                if (property == "m_LocalRotation.y")
                {
                    block->rotation.y = modification["value"].as<float>();
                }
                if (property == "m_LocalRotation.z")
                {
                    block->rotation.z = modification["value"].as<float>();
                }
                if (property == "m_LocalRotation.w")
                {
                    block->rotation.w = modification["value"].as<float>();
                }

                // Scale
                if (property == "m_LocalScale.x")
                {
                    block->scale.x = modification["value"].as<float>() / blockDefaultScale;
                }
                if (property == "m_LocalScale.y")
                {
                    block->scale.y = modification["value"].as<float>() / blockDefaultScale;
                }
                if (property == "m_LocalScale.z")
                {
                    block->scale.z = modification["value"].as<float>() / blockDefaultScale;
                }
			}

            blockPODs.emplace_back(block);
		}
	}
    catch (const YAML::Exception& e) {
        LOG_CONTENTS("YAML parsing error: " + std::string(e.what()));
        assert(true);
    }
}

void YAMLParser::ReadPartPOD(std::string_view filePath, std::vector<std::shared_ptr<PartPOD>>& partPODs)
{
    try
    {
        std::vector<YAML::Node> partNodes = YAML::LoadAllFromFile(std::string(filePath));


        for (const auto& node : partNodes)
        {
            if (!node["PrefabInstance"])
                continue;

            std::shared_ptr<PartPOD> part = std::make_shared<PartPOD>();
            for (const auto& modification : node["PrefabInstance"]["m_Modification"]["m_Modifications"])
            {
                std::string property = modification["propertyPath"].as<std::string>();
                // Type
                if (property == "m_Name")
                {
                    part->partType = GetPartType(modification["value"].as<std::string>());

                    if (part->partType == ePartType::END)
                        continue;
                }

                // Position
                if (property == "m_LocalPosition.x")
                {
                    part->position.x = modification["value"].as<float>() * 100;
                }
                if (property == "m_LocalPosition.y")
                {
                    part->position.y = modification["value"].as<float>() * 100 + BASE_HEIGHT;
                }
                if (property == "m_LocalPosition.z")
                {
                    part->position.z = modification["value"].as<float>() * 100;
                }

                // Rotation
                if (property == "m_LocalRotation.x")
                {
                    part->rotation.x = modification["value"].as<float>();
                }
                if (property == "m_LocalRotation.y")
                {
                    part->rotation.y = modification["value"].as<float>();
                }
                if (property == "m_LocalRotation.z")
                {
                    part->rotation.z = modification["value"].as<float>();
                }
                if (property == "m_LocalRotation.w")
                {
                    part->rotation.w = modification["value"].as<float>();
                }

                // Scale
                if (property == "m_LocalScale.x")
                {
                    part->scale.x = modification["value"].as<float>() / partDefaultScale;
                }
                if (property == "m_LocalScale.y")
                {
                    part->scale.y = modification["value"].as<float>() / partDefaultScale;
                }
                if (property == "m_LocalScale.z")
                {
                    part->scale.z = modification["value"].as<float>() / partDefaultScale;
                }
            }

            partPODs.emplace_back(part);
        }
    }
    catch (const YAML::Exception& e) {
        LOG_CONTENTS("YAML parsing error: " + std::string(e.what()));
        assert(true);
    }
}

eBlockType YAMLParser::GetBlockType(std::string_view blockName)
{
    std::string name = std::string(blockName);

    if(name == "1f_ingame_cube_blueemissive")
        return eBlockType::STONE_BLUE_1;
    if (name == "1f_ingame_cube2_blueemissive")
		return eBlockType::STONE_BLUE_2;
    if (name == "1f_ingame_cube3_blueemissive")
        return eBlockType::STONE_BLUE_3;
    if (name == "1f_ingame_cube_greenemissive")
        return eBlockType::STONE_GREEN_1;
    if (name == "1f_ingame_cube2_greenemissive")
        return eBlockType::STONE_GREEN_2;
    if (name == "1f_ingame_cube3_greenemissive")
        return eBlockType::STONE_GREEN_3;
    if (name == "1f_ingame_cube_yellowemissive")
        return eBlockType::STONE_YELLOW_1;
    if (name == "1f_ingame_cube2_yellowemissive")
        return eBlockType::STONE_YELLOW_2;
    if (name == "1f_ingame_cube3_yellowemissive")
        return eBlockType::STONE_YELLOW_3;
    if (name == "1f_ingame_cube_redemissive")
        return eBlockType::STONE_RED_1;
    if (name == "1f_ingame_cube2_redemissive")
        return eBlockType::STONE_RED_2;
    if (name == "1f_ingame_cube3_redemissive")
        return eBlockType::STONE_RED_3;
    //
    if (name == "2f_ingame_cube_blueemissive")
        return eBlockType::STONE_BLUE_2F;
    if (name == "2f_ingame_cube_greenemissive")
        return eBlockType::STONE_GREEN_2F;
    if (name == "2f_ingame_cube_yellowemissive")
        return eBlockType::STONE_YELLOW_2F;
    if (name == "2f_ingame_cube_redemissive")
        return eBlockType::STONE_RED_2F;
    //
    if (name == "worm2_totem_head")
        return eBlockType::TOTEM_BLUE_HEAD;
    if (name == "worm2_totem_body")
        return eBlockType::TOTEM_BLUE_BODY;
    if (name == "bird_totem_head")
        return eBlockType::TOTEM_GREEN_HEAD;
    if (name == "bird_totem_body")
        return eBlockType::TOTEM_GREEN_BODY;
    if (name == "bird_totem2_head")
        return eBlockType::TOTEM_YELLOW_HEAD;
    if (name == "bird_totem2_body")
        return eBlockType::TOTEM_YELLOW_BODY;
    if (name == "worm_totem_head")
        return eBlockType::TOTEM_RED_HEAD;
    if (name == "worm_totem_body")
        return eBlockType::TOTEM_RED_BODY;


    LOG_CONTENTS("BlockName, Type ¸ÅÄª ºÒ°¡");
    return eBlockType::END;
}

ePartType YAMLParser::GetPartType(std::string_view partName)
{
    std::string name = std::string(partName);

    if (name == "gear_fix")
    {
        return ePartType::GEAR;
    }

    LOG_CONTENTS("PartName, Type ¸ÅÄª ºÒ°¡");
    return ePartType::END;
}
