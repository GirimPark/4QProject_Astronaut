#pragma once
#include "PxPhysicsAPI.h"

/// <summary>
/// CollisionFilter
/// 충돌 필터
/// </summary>
using namespace physx;

class CollisionFilter
{
public:
    static PxFilterFlags FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize);
};