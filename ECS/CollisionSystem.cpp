#include "CollisionSystem.h"
#include "TransformSystem.h"
#include "Game.h"
#include <DirectXMath.h>

using namespace DirectX;

CollisionSystem::CollisionSystem() {
	
}

CollisionSystem::~CollisionSystem() {

}

//Generates AABBs then checks them for collisions
void CollisionSystem::Update(Game * g, float dt) {
	Collapse();
	//pre-allocate stuff
	unsigned int entityId;
	TransformSystem * ts = g->m_ts;
	BoundingBox * cc;
	vector<ComponentData> tcds = ts->GetComponentData();
	FreeVector<TransformComponent> tcs = ts->GetComponentList1();
	TransformComponent * tc;
	XMVECTOR original;
	XMVECTOR rotation;
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
		rotation = XMLoadFloat4(&tc->m_rotation);

		//reset max and min values
		max = XMLoadFloat3(&XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX));
		min = XMLoadFloat3(&XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX));

		//loop through eight bounding box points
		for (unsigned int n = 0; n < 8; n++){
			//load and rotate
			original = DirectX::XMLoadFloat3(&cc->m_bb[n]);
			original = XMVector3Rotate(original, rotation);
			//check against max and min
			max = XMVectorMax(original, max);
			min = XMVectorMin(original, min);
		}

		//store final translated max and min in aabb list
		//note that aabbIndex is used instead of c - no dead elements in the aabb list
		position = XMLoadFloat3(&tc->m_position);
		XMStoreFloat3(&m_aabbs[aabbIndex].m_max, max + position);
		XMStoreFloat3(&m_aabbs[aabbIndex].m_min, min + position);
		aabbIndex++;
	}

	//Collision checking
	//pre-allocate aabbs
	MaxMin aabb1;
	MaxMin aabb2;

	//vector to save collision pairs
	auto collisions = vector<std::pair<unsigned int, unsigned int>>();

	//iterate through all unique pairs
	for (unsigned int c = 0; aabbIndex>0 && c < aabbIndex - 1; c++) {
		aabb1 = m_aabbs[c];
		for (unsigned int n = c + 1; n < aabbIndex; n++) {
			aabb2 = m_aabbs[n];

			//add pair of indices on collision
			if (aabb1.m_max.x > aabb2.m_min.x && aabb1.m_min.x < aabb2.m_max.x
				&& aabb1.m_max.y > aabb2.m_min.y && aabb1.m_min.y < aabb2.m_max.y
				&& aabb1.m_max.z > aabb2.m_min.z && aabb1.m_min.z < aabb2.m_max.z)
				collisions.push_back(std::pair<int, int>(c, n));
		}
	}
}