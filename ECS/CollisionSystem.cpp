#include "CollisionSystem.h"
#include "TransformSystem.h"
#include "Game.h"
#include <DirectXMath.h>

using namespace DirectX;

CollisionSystem::CollisionSystem() {
	
}

CollisionSystem::~CollisionSystem() {

}

//To-do
//Fix float3/float4 conversions
void CollisionSystem::Update(Game * g, float dt) {
	//pre-allocate stuff
	unsigned int entityId;
	TransformSystem * ts = g->m_ts;
	BoundingBox * cc;
	vector<ComponentData> tcds = ts->GetComponentData();
	FreeVector<TransformComponent> tcs = ts->GetComponentList1();
	TransformComponent * tc;
	XMVECTOR rotation;
	XMVECTOR original;
	XMFLOAT3 rotatedObb[8];
	XMMATRIX modelToWorld;
	XMVECTOR max;
	XMVECTOR min;

	//size aabb list appropriately
	m_aabbs.resize(m_components.count()); //use count to avoid dead elements in aabb list
	unsigned int aabbIndex = 0; //reset index	

	//loop through bounding boxes
	unsigned int transformIndex = 0;
	for (unsigned int c = 0; c < m_components.size(); c++) {
		//skip inactives
		if (!m_componentData[c].m_active)
			continue;
		
		cc = &m_components[c]; //get component
		entityId = m_componentData[c].GetEntityId(); //get entityID

		//search for the index of this component's corresponding transform
		while (tcds[transformIndex].GetEntityId() != entityId)
		{
			transformIndex++;
			if (transformIndex == tcds.size())
				transformIndex = 0;
		}
		//save the transform and its quat
		tc = &tcs[transformIndex];
		rotation = XMLoadFloat4(&tc->m_rotation);

		//reset max and min values
		max = XMLoadFloat3(&XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX));
		min = XMLoadFloat3(&XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX));

		//loop through eight bounding box points
		for (unsigned int n = 0; n < 8; n++)
		{
			//load and rotate
			original = DirectX::XMLoadFloat3(&cc->m_bb[n]);
			original = XMVector3Rotate(original, rotation);
			//check against max and min
			max = XMVectorMax(original, max);
			min = XMVectorMin(original, min);
		}

		//store final max and min in aabb list
		//note that aabbIndex is used instead of c - no dead elements in the aabb list
		XMStoreFloat3(&m_aabbs[aabbIndex].m_max, max);
		XMStoreFloat3(&m_aabbs[aabbIndex].m_min, min);
		aabbIndex++;
	}
}