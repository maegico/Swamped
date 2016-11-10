#pragma once
#include "Camera.h"
#include "GameForwardDecl.h"
#include <array>

class Ghost {

public: 
	Ghost(unsigned int entityId, DirectX::XMFLOAT3 playerStart);
	Ghost(); 
	~Ghost();
	unsigned int m_entityId; 
	DirectX::XMFLOAT3 playerPos;
	bool phasing;	//is the ghost phasing through something
	float PHASINGSPEED;		//ghost moves slower while phasing
	float NORMALSPEED; 
	float maxSpeed;		//use to cap acceleration
	unsigned int charms[5];	//the charms that ward the ghost off

	//These variables are for steering behaviors
	DirectX::XMFLOAT3 velocity; 
	DirectX::XMFLOAT3 acceleration; 
	DirectX::XMFLOAT3 force; 

	void Update(Game * g,unsigned int playerId);
	void Seek(DirectX::XMFLOAT3 playerPosition);
	void ObstacleAvoid(); 
};