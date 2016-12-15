#include "CollisionFunctions.h"

#include "GlobalFunctions.h"
#include "Game.h"

//Returns a vector of function pointers that correspond to the given collision masks
//vector<CollisionFunction> CollisionFunctions::GetCollisionFunction(CollisionMask cm1, CollisionMask cm2) {
//	vector<CollisionFunction> returnVector;
//	if (cm1 & 1 << CollisionType::none || cm2 & 1 << CollisionType::none)
//		returnVector.push_back(&NoOpCollision);
//	return returnVector;
//}

#if BENCHMARK >= 0
//Removes both colliding entities
void CollisionFunctions::NoOpCollision(Game * g, EntityId entityId1, EntityId entityId2, float dt) {
	g->QueueRemoveEntity(entityId1);
	g->QueueRemoveEntity(entityId2);
}
#endif

void CollisionFunctions::EndState(Game * g, EntityId entityId1, EntityId entityId2, float dt)
{
	/*XMFLOAT3 newPosition = XMFLOAT3(frand2(-100, 100), 8, frand2(-100, 100));
	TransformComponent& playerTransform = g->m_transformSystem.GetComponent1(entityId2);
	playerTransform.m_position = XMFLOAT3(newPosition);
	g->m_renderingSystem.m_camera.SetPosition(newPosition);*/
	g->Quit();
}

float CollisionFunctions::frand2(float min, float max)
{
	float f = (float)rand() / RAND_MAX;
	return min + f * (max - min);
}

/*float CollisionFunctions::fRand2(float min, float max) {
	float f = (float)rand() / RAND_MAX;
	return min + f * (max - min);
}*/