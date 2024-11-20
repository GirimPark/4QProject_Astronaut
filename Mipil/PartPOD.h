#pragma once

constexpr float partDefaultScale = 100;

enum class ePartType
{
	GEAR,

	END
};

// 오브젝트 Part를 위한 POD
struct PartPOD
{
	ePartType partType;
	Vector3 position;
	Quaternion rotation;
	Vector3 scale = { 1 / partDefaultScale, 1 / partDefaultScale, 1 / partDefaultScale };
};