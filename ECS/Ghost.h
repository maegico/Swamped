#pragma once
#include "Camera.h"
#include "GameForwardDecl.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include <array>

class Ghost {

public:
	Ghost(Game * game, unsigned int entityId, DirectX::XMFLOAT3 playerStart);
	Ghost();
	~Ghost();
	unsigned int m_entityId;
	Game * g;
	DirectX::XMFLOAT3 playerPos;
	bool phasing;	//is the ghost phasing through something
	float PHASINGSPEED;		//ghost moves slower while phasing
	float NORMALSPEED;
	float maxSpeed;		//use to cap acceleration
	unsigned int charms[5];	//the charms that ward the ghost off

							//These variables are for steering behaviors
	TransformComponent * m_transform;
	PhysicsComponent * m_physics;

	XMFLOAT3 m_force = XMFLOAT3(0,0,0);

	void Update(DirectX::XMFLOAT3 playerPosition, float dt);
	void Seek(DirectX::XMFLOAT3 playerPosition);
	void ObstacleAvoid();
};