#pragma once
#include "CollisionComponent.h"
#include "CollisionFunctionTypeDef.h"
#include <vector>

class CollisionFunctions {
public:
	static vector<CollisionFunction> GetCollisionFunction(CollisionMask cm1, CollisionMask cm2);
	static vector<CollisionFunction> GetAllCollisionFunctions();

	static void NoOpCollision(Game * g, unsigned int entityId1, unsigned int entityId2, float dt);
};
