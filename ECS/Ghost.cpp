//#include "Ghost.h"
//#include "Game.h"
//
//Ghost::Ghost(unsigned int entityId, DirectX::XMFLOAT3 playerStart)
//{
//	m_entityId = entityId;
//	playerPos = playerStart;
//	phasing = false; 
//	PHASINGSPEED = 0.0002f;
//	NORMALSPEED = 0.0015f;
//	maxSpeed = NORMALSPEED; 
//}
//
//Ghost::Ghost()
//{
//
//}
//
//Ghost::~Ghost()
//{
//
//}
//
///*
//	Performs the calculations for the ghost's movement.
//*/
//void Ghost::Seek(DirectX::XMFLOAT3 playerPosition)
//{
//	playerPos = playerPosition; 
//	//we don't need to calculate y because there's no vertical movement
//	force.x = playerPos.x - ghostEntity->position.x; 
//	force.y = 0.0f; 
//	force.z = playerPos.z - ghostEntity->position.z; 
//
//	//all of this is just to normalize the vector
//	DirectX::XMVECTOR steer = DirectX::XMLoadFloat3(&force); 
//	steer = DirectX::XMVector3Normalize(steer);
//	DirectX::XMStoreFloat3(&force, steer); 
//
//	if (phasing == false)
//	{
//		acceleration.x += (force.x * NORMALSPEED);
//		acceleration.z += (force.z * NORMALSPEED);
//	}
//	else
//	{
//		acceleration.x += (force.x * PHASINGSPEED);
//		acceleration.z += (force.z * PHASINGSPEED);
//	}
//}
//
///*
//	This single method should be called during the game's update phase.
//	It calls all other necessary methods and handles ghost movement.
//	@param: Requires the player's current position to be passed
//*/
//void Ghost::Update(Game * g, unsigned int playerId)
//{
//	PhysicsComponent & myPhysics = g->m_ts.GetComponent2(m_entityId);
//	TransformComponent & playerTransform = g->m_ts.GetComponent1(playerId);
//	Seek(playerTransform.m_position);
//	//ObstacleAvoid(); 
//	
//	//velocity.x += acceleration.x; 
//	//velocity.y = 0.0f; 
//	//velocity.z += acceleration.z; 
//
//	DirectX::XMVECTOR steer = DirectX::XMLoadFloat3(&myPhysics.m_velocity);
//	float magnitude = DirectX::XMVectorGetX(DirectX::XMVector3Length(steer));
//	if (magnitude > maxSpeed)
//	{
//		steer = DirectX::XMVector3Normalize(steer);
//		DirectX::XMStoreFloat3(&myPhysics.m_velocity, steer);
//		//velocity.x *= NORMALSPEED; 
//		//velocity.z *= NORMALSPEED; 
//	}
//	
//	ghostEntity->MoveBy(velocity); 
//	acceleration.x = 0.0f; 
//	acceleration.y = 0.0f; 
//	acceleration.z = 0.0f; 
//
//	
//}
//
///*
//	Causes the ghost to avoid the charms if it gets near them.
//	Fully implimented, but charms are not, so do not call yet
//*/
//void Ghost::ObstacleAvoid()
//{
//	float strength; 
//	for (int i = 0; i < sizeof(charms); i++)
//	{
//		DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&ghostEntity->position);
//		DirectX::XMVECTOR charmPos = DirectX::XMLoadFloat3(&charms[i]->position); 
//		DirectX::XMVECTOR distance = DirectX::XMVectorSubtract(charmPos, position); 
//
//		strength = abs(DirectX::XMVectorGetX(DirectX::XMVector3Length(distance)));
//
//		if (strength < 2.0f)
//		{
//			distance = DirectX::XMVectorScale(distance, (1.0f / strength));
//			DirectX::XMStoreFloat3(&force, distance); 
//			acceleration.x += force.x; 
//			acceleration.z += force.z;
//		}
//	}
//}
//
