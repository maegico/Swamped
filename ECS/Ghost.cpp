#include "Ghost.h"
#include "Game.h"

Ghost::Ghost(Game * game, unsigned int entityId, DirectX::XMFLOAT3 playerStart)
{
	m_entityId = entityId;
	playerPos = playerStart;
	phasing = false;
	PHASINGSPEED = 0.0002f;
	NORMALSPEED = 1.0f;
	maxSpeed = NORMALSPEED;
	g = game;
	//position = g->m_transformSystem.GetComponent1(entityId).m_position;
	m_transform = &game->m_transformSystem.GetComponent1(m_entityId);
	m_physics = &game->m_transformSystem.GetComponent2(m_entityId);
}

Ghost::Ghost()
{

}

Ghost::~Ghost()
{

}

/*
Performs the calculations for the ghost's movement.
*/
void Ghost::Seek(DirectX::XMFLOAT3 playerPosition)
{
	playerPos = playerPosition;
	//we don't need to calculate y because there's no vertical movement
	XMFLOAT3 force = XMFLOAT3(playerPos.x - m_transform->m_position.x, 0, playerPos.z - m_transform->m_position.z);

	//all of this is just to normalize the vector
	DirectX::XMVECTOR steer = DirectX::XMLoadFloat3(&force);
	steer = DirectX::XMVector3Normalize(steer);
	DirectX::XMStoreFloat3(&m_force, steer);

	if (phasing == false)
	{
		m_physics->m_acceleration.x += (force.x * NORMALSPEED);
		m_physics->m_acceleration.z += (force.z * NORMALSPEED);
	}
	else
	{
		m_physics->m_acceleration.x += (force.x * PHASINGSPEED);
		m_physics->m_acceleration.z += (force.z * PHASINGSPEED);
	}
}

/*
This single method should be called during the game's update phase.
It calls all other necessary methods and handles ghost movement.
@param: Requires the player's current position to be passed
*/
void Ghost::Update(DirectX::XMFLOAT3 playerPosition, float dt)
{
	//PhysicsComponent & myPhysics = g->m_transformSystem.GetComponent2(m_entityId);
	//TransformComponent & playerTransform = g->m_ts.GetComponent1(playerId);
	//TransformComponent & myTransform = g->m_transformSystem.GetComponent1(m_entityId);
	Seek(playerPosition);
	//ObstacleAvoid(); 

	//m_physics->m_velocity.x += m_physics->m_acceleration.x;
	//m_physics->m_velocity.y = 0.0f;
	//m_physics->m_velocity.z += m_physics->m_acceleration.z;

	DirectX::XMVECTOR steer = DirectX::XMLoadFloat3(&m_physics->m_velocity);

	float magnitude = DirectX::XMVectorGetX(DirectX::XMVector3Length(steer));
	if (magnitude > maxSpeed)
	{
		steer = DirectX::XMVector3Normalize(steer);
		steer *= maxSpeed;
		DirectX::XMStoreFloat3(&m_physics->m_velocity, steer);
		//velocity.x *= maxSpeed; 
		//velocity.z *= maxSpeed; 
	}

	//ghostEntity->MoveBy(velocity); 
	//position.x += velocity.x;
	//position.z += velocity.z;
	//steer = DirectX::XMLoadFloat3(&position);
	//DirectX::XMStoreFloat3(&myTransform.m_position, steer);
	//myTransform.m_position = position;
	//acceleration.x = 0.0f;
	//acceleration.y = 0.0f;
	//acceleration.z = 0.0f;

	XMVECTOR forward = XMLoadFloat3(&XMFLOAT3(1, 0, 0));
	XMVECTOR up = XMLoadFloat3(&XMFLOAT3(0, 1, 0));

	//steer = DirectX::XMVector3Normalize(steer);

	DirectX::XMFLOAT3 turnTo;
	XMVECTOR dot = DirectX::XMVector3Dot(forward, steer);
	DirectX::XMStoreFloat3(&turnTo, dot);
	float angle = 0.0f;
	if (turnTo.x < 0.0f)
		angle = acos(turnTo.x);
	else
		angle = -1 * acos(turnTo.x);

	//DirectX::XMStoreFloat3(&turnTo, DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)), steer));
	//float angle = acos(turnTo.x);

	


	//XMStoreFloat4(&m_transform->m_rotation, XMQuaternionRotationAxis(up, XMVectorGetIntX(XMVector3AngleBetweenVectors(forward, XMLoadFloat3(&m_physics->m_velocity)))));
	DirectX::XMStoreFloat4(&m_transform->m_rotation,DirectX::XMQuaternionRotationAxis(up, angle));
}

/*
Causes the ghost to avoid the charms if it gets near them.
Fully implimented, but charms are not, so do not call yet
*/
void Ghost::ObstacleAvoid()
{
	float strength;
	/*for (int i = 0; i < sizeof(charms); i++)
	{
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&ghostEntity->position);
	DirectX::XMVECTOR charmPos = DirectX::XMLoadFloat3(&charms[i]->position);
	DirectX::XMVECTOR distance = DirectX::XMVectorSubtract(charmPos, position);

	strength = abs(DirectX::XMVectorGetX(DirectX::XMVector3Length(distance)));

	if (strength < 2.0f)
	{
	distance = DirectX::XMVectorScale(distance, (1.0f / strength));
	DirectX::XMStoreFloat3(&force, distance);
	acceleration.x += force.x;
	acceleration.z += force.z;
	}
	}*/
}

