#include "CollisionSystem.h"
#include "TransformSystem.h"
#include "Game.h"
#include "CollisionFunctions.h"

using namespace DirectX;

CollisionMask MakeCollisionMask(vector<CollisionType> types) {
	CollisionMask mask;

	for (auto type : types) {
		mask = mask | 1 << type;
	}

	return mask;
}

CollisionSystem::CollisionSystem() {
	
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
	TransformSystem * ts = g->m_ts;
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
	XMVECTOR position;

	//size aabb list appropriately
	m_aabbs.resize(m_components.count()); //use count to avoid dead elements in aabb list
	unsigned int aabbIndex = 0; //reset index	

	//loop through bounding boxes
	unsigned int transformIndex = 0;
	for (unsigned int c = 0; c < m_collapsedCount; c++) {
		//skip inactives
		//if (!m_componentData[c].m_active)
		//	continue;

		cc = &m_collapsedComponents[c]; //get component
		entityId = m_componentData[c].GetEntityId(); //get entityID

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
		XMStoreFloat3(&m_aabbs[c].m_max, max);
		XMStoreFloat3(&m_aabbs[c].m_min, min);
		m_aabbs[c].m_cm = cc->m_cm;
	}

	//Collision checking

	//clear collision map
	for (auto& kv : m_collisionMap) {
		kv.second.clear();
	}

	//iterate through all unique pairs
	parallel_for(size_t(0), m_collapsedCount - 1, [&](unsigned int c) {
		MaxMin aabb1 = m_aabbs[c];
		MaxMin aabb2;
		for (unsigned int n = c + 1; n < m_collapsedCount; n++) {
			aabb2 = m_aabbs[n];

			//add pair of indices on collision
			if (aabb1.m_max.x > aabb2.m_min.x && aabb1.m_min.x < aabb2.m_max.x
				&& aabb1.m_max.y > aabb2.m_min.y && aabb1.m_min.y < aabb2.m_max.y
				&& aabb1.m_max.z > aabb2.m_min.z && aabb1.m_min.z < aabb2.m_max.z)
			{
				//Get correct collision function and emplace it into the collision map along with an empty lock vector
				vector<CollisionFunction> cfs = CollisionFunctions::GetCollisionFunction(aabb1.m_cm, aabb2.m_cm);
				for (auto cf : cfs)
				{
					auto pair = std::make_pair(cf, LockVector<std::pair<unsigned int, unsigned int>>());
					m_collisionMap.emplace(pair);
					//push the entityIDs to the vector under this collision function
					m_collisionMap[pair.first].push_back(std::make_pair(c, n));
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