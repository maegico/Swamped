#include "CollisionSystem.h"
#include <DirectXMath.h>

using namespace DirectX;

CollisionSystem::CollisionSystem() {
	
}

//To-do
//Fix float3/float4 conversions
void CollisionSystem::Update(Game * g) {
	//pre-allocate component, obb, and world matrix
	CollisionComponent * cc;
	XMVECTOR obb[8];
	XMMATRIX modelToWorld;

	//size aabb list appropriately
	m_aabbs.resize(m_components.size());

	//loop through obbs
	for (unsigned int c = 0; c < m_components.size(); c++) {
		cc = &m_components[c].GetData(); //get component data

		//To-do
		//load world matrix from transform system

		//loop through eight bounding box points
		for (unsigned int n = 0; n < 8; n++)
		{
			//store obb point transformed by world matrix into aabb list
			XMStoreFloat3(&m_aabbs[n][c], XMVector3Transform(XMLoadFloat3(&cc->m_Obb[c]), modelToWorld));
		}
	}
}