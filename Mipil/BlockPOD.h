#pragma once

constexpr float blockDefaultScale = 100;

enum class eBlockType
{
	STONE_BLUE_1,		// 1f_ingame_cube_blueemissive
	STONE_BLUE_2,		// 1f_ingame_cube2_blueemissive
	STONE_BLUE_3,		// 1f_ingame_cube3_blueemissive
	STONE_GREEN_1,		// 1f_ingame_cube_greenemissive
	STONE_GREEN_2,		// 1f_ingame_cube2_greenemissive
	STONE_GREEN_3,		// 1f_ingame_cube3_greenemissive
	STONE_YELLOW_1,		// 1f_ingame_cube_yellowemissive
	STONE_YELLOW_2,		// 1f_ingame_cube2_yellowemissive
	STONE_YELLOW_3,		// 1f_ingame_cube3_yellowemissive
	STONE_RED_1,		// 1f_ingame_cube_redemissive
	STONE_RED_2,		// 1f_ingame_cube2_redemissive
	STONE_RED_3,		// 1f_ingame_cube3_redemissive

	STONE_BLUE_2F,		// 2f_ingame_cube_blueemissive
	STONE_GREEN_2F,		// 2f_ingame_cube_greenemissive
	STONE_YELLOW_2F,	// 2f_ingame_cube_yellowemissive
	STONE_RED_2F,		// 2f_ingame_cube_redemissive

	TOTEM_BLUE_HEAD,	// worm2_totem_head
	TOTEM_BLUE_BODY,	// worm2_totem_body
	TOTEM_GREEN_HEAD,	// bird_totem_head
	TOTEM_GREEN_BODY,	// bird_totem_body
	TOTEM_YELLOW_HEAD,	// bird_totem2_head
	TOTEM_YELLOW_BODY,	// bird_totem2_body
	TOTEM_RED_HEAD,		// worm_totem_head
	TOTEM_RED_BODY,		// worm_totem_body

	END
};

// 오브젝트 Block을 위한 POD
struct BlockPOD
{
    eBlockType blockType;
    Vector3 position;
    Quaternion rotation;
    Vector3 scale = { 1 / blockDefaultScale, 1 / blockDefaultScale, 1 / blockDefaultScale };
};