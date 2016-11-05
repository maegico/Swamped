#pragma once
#include "CollisionComponent.h"
#include <vector>
typedef void(*CollisionFunction)(unsigned int entityId1, unsigned int entityId2, float dt);
class CollisionFunctions {
public:
	static vector<CollisionFunction> GetCollisionFunction(CollisionMask cm1, CollisionMask cm2);

	static void NoOpCollision(unsigned int entityId1, unsigned int entityId2, float dt);
};
