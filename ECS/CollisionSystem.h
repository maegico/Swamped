#pragma once
#include "System.h"
#include "CollisionComponent.h"
#include "CollisionFunctionTypeDef.h"
#include "LockVector.h"
#include "ClearVector.h"
#include "ClearArray.h"
#include "EntityIdTypeDef.h"
#include <DirectXMath.h>
#include <mutex>
#include <ppl.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
using namespace DirectX;
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
	vector<CollapsedComponent<TypedMaxMin>> m_aabbs;
	mutex m_collisionsMutex;
	unordered_map<CollisionFunction, LockVector<pair<EntityId, EntityId>>> m_collisionMap;
	ClearVector<vector<ClearVector<CollapsedComponent<MaxMin>>>> m_spatialHashGrid;
	// m_mapMin;
	//XMFLOAT3 m_cellDimensions;
	XMFLOAT3 m_cellCounts;
	//ClearVector<pair<CollapsedComponent<MaxMin>, ClearArray<8,unsigned int>>> m_cellCrossers;
	ClearVector<ClearVector<EntityId>> m_registeredCollisions;
	vector<tuple<CollisionType, CollisionType, CollisionFunction>> m_collisionFunctions;
};