#pragma once
#include "CollisionComponent.h"
#include <vector>

typedef void(*CollisionFunction)(unsigned int entityId1, unsigned int entityId2, float dt);

class CollisionFunctions {
public:
	static vector<CollisionFunction> GetCollisionFunction(CollisionMask cm1, CollisionMask cm2){
		vector<CollisionFunction> returnVector;
		if (cm1 & 1 << CollisionType::none || cm2 & 1 << CollisionType::none)
			returnVector.push_back(&NoOpCollision);
		return returnVector;
	}

	static void NoOpCollision(unsigned int entityId1, unsigned int entityId2, float dt) {  };
};
