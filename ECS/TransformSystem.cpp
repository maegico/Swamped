#include "TransformSystem.h"
#include "Constructors.h"
void TransformSystem::Update(Game * game, float dt) {
	StartTimer();
	/*for (unsigned int c = 0; c < m_components1.size(); c++) {
		if (m_componentData[c].m_active && rand() % 50 < 1)
			g->QueueRemoveEntity(m_componentData[c].GetEntityId());
	}*/
	Collapse();
	//Movement
	//Pre-allocate stuff
	
	//loop through all components
	parallel_for(size_t(0), m_collapsedCount, [&](unsigned int c) {
		XMVECTOR position;
		XMVECTOR rotation;
		XMVECTOR velocity;
		XMVECTOR acceleration;
		//XMVECTOR spin;
		XMVECTOR rotationalVelocity;
		//XMVECTOR rotationalAcceleration;
		//load stuff
		position = XMLoadFloat3(&m_collapsedComponents1[c].m_component.m_position);
		rotation = XMLoadFloat4(&m_collapsedComponents1[c].m_component.m_rotation);
		velocity = XMLoadFloat3(&m_collapsedComponents2[c].m_component.m_velocity);
		acceleration = XMLoadFloat3(&m_collapsedComponents2[c].m_component.m_acceleration);
		if(m_collapsedComponents2[c].m_component.m_gravity)
			acceleration = XMVectorAdd(acceleration, XMLoadFloat3(&XMFLOAT3(0, m_gravity, 0)));
		rotationalVelocity = XMLoadFloat3(&m_collapsedComponents2[c].m_component.m_rotationalVelocity);
		if (XMVectorGetX(XMVector3Length(rotationalVelocity)) != 0)
		{
			//spin = XMVectorScale(XMQuaternionMultiply(XMLoadFloat4(&XMFLOAT4(0, rotationalVelocity.x, rotationalVelocity.y, rotationalVelocity.z)), rotation), .5f);
			rotation = XMQuaternionSlerp(rotation, XMQuaternionMultiply(rotation, XMQuaternionRotationAxis(rotationalVelocity, XMVectorGetX(XMVector3Length(rotationalVelocity)))), dt);
		}
		//rotationalVelocity = XMLoadFloat4(&m_components2[c].m_rotationalVelocity);//XMLoadFloat4(&m_components2[c].m_rotationalVelocity);
		//rotationalAcceleration = XMLoadFloat4(&m_components2[c].m_rotationalAcceleration);//XMLoadFloat4(&m_components2[c].m_rotationalAcceleration);

		//do maths
		velocity += dt*acceleration;
		position += dt*velocity;
		//rotationalVelocity += dt*rotationalAcceleration;
		//rotation += dt*rotationalVelocity;
		//rotationalMax = XMQuaternionMultiply(rotationalVelocity, rotationalAcceleration);
		//rotationalVelocity = XMQuaternionNormalize(XMQuaternionSlerp(rotationalVelocity, rotationalMax, dt));
		//rotationMax = XMQuaternionMultiply(rotation, rotationalVelocity);
		//rotation = XMQuaternionSlerp(rotation, rotationMax, dt);

		//store stuff
		XMStoreFloat3(&m_components2[m_collapsedComponents2[c].m_handle].m_velocity, velocity);
		XMStoreFloat3(&m_components1[m_collapsedComponents1[c].m_handle].m_position, position);
		XMStoreFloat3(&m_components2[m_collapsedComponents2[c].m_handle].m_rotationalVelocity, rotationalVelocity);
		//m_components2[c].m_rotationalVelocity = rotationalVelocity;
		XMStoreFloat4(&m_components1[m_collapsedComponents1[c].m_handle].m_rotation, rotation);
	});
	StopTimer();
}

//Returns a matrix generated from the given component's properties
XMMATRIX TransformSystem::GetMatrix(TransformComponent& tc) {
	return XMMatrixMultiply(XMMatrixRotationQuaternion(XMLoadFloat4(&tc.m_rotation)), XMMatrixTranslationFromVector(XMLoadFloat3(&tc.m_position)));
}