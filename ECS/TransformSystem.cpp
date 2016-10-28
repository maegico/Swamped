#include "TransformSystem.h"
#include "Constructors.h"
#include <DirectXMath.h>
using namespace DirectX;
void TransformSystem::Update(Game * g) {
	for (unsigned int c = 0; c < m_components1.size(); c++) {
		if (m_componentData[c].m_active && rand() % 2 == 1)
			g->RemoveEntity(m_componentData[c].GetEntityId());
	}
	unsigned int newComponents = rand() % 100000;
	for (unsigned int c = 0; c < newComponents; c++)
	{
		Constructors::CreateTransform(g, {}, {});
	}

	//Movement
	//Pre-allocate stuff
	XMVECTOR position;
	XMVECTOR velocity;
	XMVECTOR acceleration;
	//loop through all components
	for (unsigned int c = 0; c < m_components1.size(); c++) {
		if (m_componentData[c].m_active) //only do for active components
		{
			//load position, velocity, acceleration
			position = XMLoadFloat3(&m_components1[c].m_position);
			velocity = XMLoadFloat3(&m_components2[c].m_velocity);
			acceleration = XMLoadFloat3(&m_components2[c].m_acceleration);
			//do maths
			velocity += acceleration;
			position += velocity;
			//store velocity and position
			XMStoreFloat3(&m_components2[c].m_velocity, velocity);
			XMStoreFloat3(&m_components1[c].m_position, position);
		}
	}
}

size_t TransformSystem::GetSize() {
	return m_components1.size();
}

size_t TransformSystem::GetCount() {
	return m_components1.count();
}