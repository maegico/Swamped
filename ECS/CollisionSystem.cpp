#include "CollisionSystem.h"
#include "TransformSystem.h"
#include "Game.h"
#include "CollisionFunctions.h"

#define CELL_DIVISIONS 9

using namespace DirectX;

CollisionMask MakeCollisionMask(vector<CollisionType> types) {
	CollisionMask mask;

	for (auto type : types) {
		mask = mask | 1 << type;
	}

	return mask;
}

CollisionSystem::CollisionSystem() {
	auto cfs = CollisionFunctions::GetAllCollisionFunctions();
	for (auto cf : cfs) {
		m_collisionMap.emplace(pair<CollisionFunction, LockVector<pair<unsigned int, unsigned int>>>(cf, LockVector<pair<unsigned int, unsigned int>>()));
	}
}

CollisionSystem::~CollisionSystem() {

}

//Generates AABBs then checks them for collisions
void CollisionSystem::Update(Game * g, float dt) {
	Collapse();
	if (m_collapsedCount == 0)
		return;
	//pre-allocate stuff
	unsigned int entityId;
	TransformSystem * ts = &g->m_ts;
	BoundingBox * cc;
	vector<ComponentData> & tcds = ts->GetComponentData();
	FreeVector<TransformComponent> & tcs = ts->GetComponentList1();
	TransformComponent * tc;
	XMVECTOR original;
	//XMVECTOR rotation;
	XMFLOAT3 rotatedObb[8];
	XMMATRIX modelToWorld;
	XMVECTOR max;
	XMVECTOR min;
	XMVECTOR globalMax = XMLoadFloat3(&XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX));
	XMVECTOR globalMin = XMLoadFloat3(&XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX));
	XMVECTOR globalPad = XMLoadFloat3(&XMFLOAT3(1, 1, 1));
	//XMVECTOR position;

	//size aabb list appropriately
	m_aabbs.resize(m_components.count()); //use count to avoid dead elements in aabb list
	unsigned int aabbIndex = 0; //reset index	

	//loop through bounding boxes
	unsigned int transformIndex = 0;
	for (unsigned int c = 0; c < m_collapsedCount; c++) {
		//skip inactives
		//if (!m_componentData[c].m_active)
		//	continue;

		cc = &m_collapsedComponents[c].m_component; //get component
		entityId = m_collapsedComponents[c].m_entityId; //get entityID

		//search for the index of this component's corresponding transform
		ts->SearchForEntityId(transformIndex, entityId);

		//save the transform and its quat
		tc = &tcs[transformIndex];
		//rotation = XMLoadFloat4(&tc->m_rotation);
		modelToWorld = TransformSystem::GetMatrix(*tc);

		//reset max and min values
		max = XMLoadFloat3(&XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX));
		min = XMLoadFloat3(&XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX));

		//loop through eight bounding box points
		for (unsigned int n = 0; n < 8; n++) {
			//load and translate to world space
			original = DirectX::XMLoadFloat3(&cc->m_bb[n]);
			original = XMVector3Transform(original, modelToWorld);
			//check against max and min
			max = XMVectorMax(original, max);
			min = XMVectorMin(original, min);
		}
		float distanceFromGround = XMVectorGetY(min) - 0;
		if (distanceFromGround < 0)
		{
			XMVECTOR offset = XMLoadFloat3(&XMFLOAT3(0, -distanceFromGround, 0));
			max = XMVectorAdd(max, offset);
			min = XMVectorAdd(min, offset);
			XMStoreFloat3(&(tc->m_position), XMVectorAdd(XMLoadFloat3(&tc->m_position), offset));
			ts->GetComponentList2()[transformIndex].m_velocity.y = 0;
		}
		//store final translated max and min in aabb list
		//position = XMLoadFloat3(&tc->m_position);
		XMStoreFloat3(&m_aabbs[c].m_component.m_max, max);
		XMStoreFloat3(&m_aabbs[c].m_component.m_min, min);
		globalMax = XMVectorMax(globalMax, max);
		globalMin = XMVectorMin(globalMin, min);
		m_aabbs[c].m_component.m_cm = cc->m_cm;
		m_aabbs[c].m_entityId = entityId;
		m_aabbs[c].m_handle = m_collapsedComponents[c].m_handle;
	}

	globalMax = XMVectorAdd(globalMax, globalPad);
	globalMin = XMVectorSubtract(globalMin, globalPad);

	//prep spatial hash grid
	//XMStoreFloat3(&m_mapMin, globalMin);
	XMVECTOR dimensions = XMVectorSubtract(globalMax, globalMin);
	dimensions = XMVectorScale(dimensions, 1.0f/CELL_DIVISIONS);
	//XMStoreFloat3(&m_cellDimensions, dimensions);
	m_cellCounts = XMFLOAT3(CELL_DIVISIONS, CELL_DIVISIONS, CELL_DIVISIONS);
	for (unsigned int c = 0; c < m_spatialHashGrid.size(); c++)
		m_spatialHashGrid[c].clear();
	m_spatialHashGrid.resize(m_cellCounts.x * m_cellCounts.y * m_cellCounts.z);
	m_cellCrossers.clear();

	//populate spatial hash grid
	parallel_for(size_t(0), m_collapsedCount, [&](unsigned int c) {
		MaxMin aabb = m_aabbs[c].m_component;
		XMFLOAT3 bb[8] = {
			{aabb.m_max.x, aabb.m_max.y, aabb.m_max.z},
			{ aabb.m_max.x, aabb.m_max.y, aabb.m_min.z },
			{ aabb.m_max.x, aabb.m_min.y, aabb.m_max.z },
			{ aabb.m_max.x, aabb.m_min.y, aabb.m_min.z },
			{ aabb.m_min.x, aabb.m_max.y, aabb.m_max.z },
			{ aabb.m_min.x, aabb.m_max.y, aabb.m_min.z },
			{ aabb.m_min.x, aabb.m_min.y, aabb.m_max.z },
			{ aabb.m_min.x, aabb.m_min.y, aabb.m_min.z }
		};
		XMVECTOR pointxm;
		ClearArray<8, unsigned int> gridIndices;
		for (auto& point : bb) {
			pointxm = XMLoadFloat3(&point);
			XMStoreFloat3(&point, XMVectorFloor(XMVectorDivide(XMVectorSubtract(pointxm, globalMin), dimensions)));
			gridIndices.push(point.z * m_cellCounts.x * m_cellCounts.y + point.y * m_cellCounts.x + point.x, true);
		}
		for(unsigned int n = 0;n<gridIndices.size();n++)
			m_spatialHashGrid[gridIndices[n]].add(m_aabbs[c]);
	});

	//clear collision map
	for (auto& kv : m_collisionMap) {
		kv.second.clear();
	}
	m_registeredCollisions.resize(m_componentData.size());
	for (auto& cl : m_registeredCollisions)
		cl.clear();

	//check collisions
	parallel_for(size_t(0), m_spatialHashGrid.size(), [&](unsigned int b) {
		auto bucketCv = m_spatialHashGrid[b];
		if (bucketCv.size() == 0)
			return;
		for (unsigned int c = 0; c < bucketCv.size()-1; c++) {
			CollapsedComponent<MaxMin> caabb1 = bucketCv[c];
			MaxMin aabb1 = caabb1.m_component;
			CollapsedComponent<MaxMin> caabb2;
			MaxMin aabb2;
			//pair<unsigned int, unsigned int> p = (caabb1.m_entityId < caabb2.m_entityId) ? std::make_pair(caabb1.m_entityId, caabb2.m_entityId) : std::make_pair(caabb2.m_entityId, caabb1.m_entityId);
			for (unsigned int n = c + 1; n < bucketCv.size(); n++) {
				caabb2 = bucketCv[n];
				aabb2 = caabb2.m_component;

				//add pair of indices on collision
				if (aabb1.m_max.x > aabb2.m_min.x && aabb1.m_min.x < aabb2.m_max.x
					&& aabb1.m_max.y > aabb2.m_min.y && aabb1.m_min.y < aabb2.m_max.y
					&& aabb1.m_max.z > aabb2.m_min.z && aabb1.m_min.z < aabb2.m_max.z
					&& !m_registeredCollisions[caabb1.m_handle].contains(caabb2.m_entityId)
					&& !m_registeredCollisions[caabb2.m_handle].contains(caabb1.m_entityId))
				{
					m_registeredCollisions[caabb1.m_handle].add(caabb2.m_entityId);
					m_registeredCollisions[caabb2.m_handle].add(caabb1.m_entityId);
					//Get correct collision function and emplace it into the collision map along with an empty lock vector
					vector<CollisionFunction> cfs = CollisionFunctions::GetCollisionFunction(aabb1.m_cm, aabb2.m_cm);
					for (auto cf : cfs)
					{
						//auto pair = std::make_pair(cf, LockVector<std::pair<unsigned int, unsigned int>>());
						//m_collisionMap.emplace(pair);
						//push the entityIDs to the vector under this collision function
						m_collisionMap[cf].push_back(std::make_pair(bucketCv[c].m_entityId, bucketCv[n].m_entityId));
					}
				}
			}
		}
	});
	
	//loop through all collision functions
	for (auto& kv : m_collisionMap) {
		auto collisions = kv.second; //get list of actual collisions
		for (unsigned int c = 0; c < collisions.size(); c++) {
			//pass both entityIDs and dT to collision function
			kv.first(g, collisions[c].first, collisions[c].second, dt);
		}
	}
}