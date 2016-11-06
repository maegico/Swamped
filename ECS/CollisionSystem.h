#pragma once
#include "System.h"
#include "CollisionComponent.h"
#include "CollisionFunctionTypeDef.h"
#include "LockVector.h"
#include "ClearVector.h"
#include "ClearArray.h"
#include <DirectXMath.h>
#include <mutex>
#include <ppl.h>
#include <map>

using namespace Concurrency;
//A System implementation
class CollisionSystem : public System<BoundingBox> {
public:
	//Generates AABBs and checks collision
	void Update(Game * g, float dT);
	CollisionSystem();
	~CollisionSystem();
private:
	//Pre-allocated list of the current frame's AABBs
	vector<MaxMin> m_aabbs;
	mutex m_collisionsMutex;
	map<CollisionFunction, LockVector<pair<unsigned int, unsigned int>>> m_collisionMap;
};