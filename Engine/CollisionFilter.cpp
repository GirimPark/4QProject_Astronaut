#include "pch.h"
#include "CollisionFilter.h"
#include "GameObject.h"

#define TYPE_BIT(type) (1 << static_cast<PxU32>(type))


PxFilterFlags CollisionFilter::FilterShader(PxFilterObjectAttributes attributes0, PxFilterData filterData0, PxFilterObjectAttributes attributes1, PxFilterData filterData1, PxPairFlags& pairFlags, const void* constantBlock, PxU32 constantBlockSize)
{
	{
		PxU32 group0 = filterData0.word0;
		PxU32 group1 = filterData1.word0;

		if (PxFilterObjectIsTrigger(attributes0) || PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = PxPairFlag::eTRIGGER_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_PERSISTS;
			return PxFilterFlags();
		}

		pairFlags = PxPairFlag::eCONTACT_DEFAULT | PxPairFlag::eNOTIFY_TOUCH_FOUND | PxPairFlag::eNOTIFY_TOUCH_PERSISTS | PxPairFlag::eNOTIFY_TOUCH_LOST;;

		if ((group0 == TYPE_BIT(eObjectType::PLAYER) && group1 == TYPE_BIT(eObjectType::LEVEL)) ||
			(group0 == TYPE_BIT(eObjectType::LEVEL)) && group1 == TYPE_BIT(eObjectType::PLAYER))
		{
			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
			return PxFilterFlag::eDEFAULT;
		}

		if ((group0 == TYPE_BIT(eObjectType::PLAYER) && group1 == TYPE_BIT(eObjectType::PLAYER)))
		{
			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
			return PxFilterFlag::eDEFAULT;
		}

		if ((group0 == TYPE_BIT(eObjectType::PLAYER) && group1 == TYPE_BIT(eObjectType::HOLLOWBOX)) ||
			(group0 == TYPE_BIT(eObjectType::HOLLOWBOX) && group1 == TYPE_BIT(eObjectType::PLAYER)))
		{
			pairFlags |= PxPairFlag::eNOTIFY_CONTACT_POINTS;
			return PxFilterFlag::eDEFAULT;
		}

		return PxFilterFlags();
	}
}
