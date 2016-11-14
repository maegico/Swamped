#pragma once
#include "CollisionComponent.h"
#include "CollisionFunctionTypeDef.h"
#include "EntityIdTypeDef.h"
#include <vector>

class CollisionFunctions {
public:
	static vector<CollisionFunction> GetCollisionFunction(CollisionMask cm1, CollisionMask cm2);
	static vector<CollisionFunction> GetAllCollisionFunctions();

	static void NoOpCollision(Game * g, EntityId entityId1, EntityId entityId2, float dt);
};
