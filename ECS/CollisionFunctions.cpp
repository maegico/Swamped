#include "CollisionFunctions.h"
#include "Game.h"

vector<CollisionFunction> CollisionFunctions::GetCollisionFunction(CollisionMask cm1, CollisionMask cm2) {
	vector<CollisionFunction> returnVector;
	if (cm1 & 1 << CollisionType::none || cm2 & 1 << CollisionType::none)
		returnVector.push_back(&NoOpCollision);
	return returnVector;
}

void CollisionFunctions::NoOpCollision(Game * g, unsigned int entityId1, unsigned int entityId2, float dt) {
	g->QueueRemoveEntity(entityId1);
	g->QueueRemoveEntity(entityId2);
}