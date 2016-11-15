#include "CollisionFunctions.h"
#include "Game.h"

vector<CollisionFunction> CollisionFunctions::GetCollisionFunction(CollisionMask cm1, CollisionMask cm2) {
	vector<CollisionFunction> returnVector;
	if (cm1 & 1 << CollisionType::none || cm2 & 1 << CollisionType::none)
		returnVector.push_back(&NoOpCollision);
	return returnVector;
}

vector<CollisionFunction> CollisionFunctions::GetAllCollisionFunctions() {
	return{
		&NoOpCollision
	};
}

void CollisionFunctions::NoOpCollision(Game * g, EntityId entityId1, EntityId entityId2, float dt) {
	g->QueueRemoveEntity(entityId1);
	g->QueueRemoveEntity(entityId2);
}